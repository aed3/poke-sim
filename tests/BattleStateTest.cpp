#include "Tests.hpp"

namespace pokesim {
struct IdealPP_MaxPP {
  std::uint8_t pp;
  std::uint8_t maxPP;
};

void checkCreatedPokemon(const entt::handle& truth, const Simulation::PokemonCreationInfo& ideal) {
  REQUIRE(truth.all_of<ID, Side, Battle, SpeciesName, AbilityName, Level, MoveSlots, EVs, IVs>());
  REQUIRE(truth.all_of<stat::HP, stat::Atk, stat::Def, stat::Spa, stat::Spd, stat::Spe>());

  if (ideal.id) REQUIRE(truth.get<ID>().id == ideal.id);
  else REQUIRE(truth.get<ID>().id != 0);

  REQUIRE(truth.get<SpeciesName>().name == ideal.species);
  REQUIRE(truth.get<AbilityName>().name == ideal.ability);
  REQUIRE(truth.get<Level>().level == ideal.level);

  if (ideal.item == dex::NO_ITEM) REQUIRE_FALSE(truth.any_of<ItemName>());
  else REQUIRE(truth.get<ItemName>().name == ideal.item);

  if (ideal.gender == dex::NO_GENDER) REQUIRE_FALSE(truth.any_of<GenderName>());
  else REQUIRE(truth.get<GenderName>().name == ideal.gender);

  if (ideal.status == dex::NO_STATUS) REQUIRE_FALSE(truth.any_of<StatusName>());
  else REQUIRE(truth.get<StatusName>().name == ideal.status);

  if (ideal.nature == dex::NO_NATURE) REQUIRE_FALSE(truth.any_of<NatureName>());
  else REQUIRE(truth.get<NatureName>().name == ideal.nature);

  REQUIRE(truth.get<stat::HP>().stat == ideal.stats.hp);
  REQUIRE(truth.get<stat::Atk>().stat == ideal.stats.atk);
  REQUIRE(truth.get<stat::Def>().stat == ideal.stats.def);
  REQUIRE(truth.get<stat::Spa>().stat == ideal.stats.spa);
  REQUIRE(truth.get<stat::Spd>().stat == ideal.stats.spd);
  REQUIRE(truth.get<stat::Spe>().stat == ideal.stats.spe);

  REQUIRE(truth.get<EVs>().hp == ideal.evs.hp);
  REQUIRE(truth.get<EVs>().atk == ideal.evs.atk);
  REQUIRE(truth.get<EVs>().def == ideal.evs.def);
  REQUIRE(truth.get<EVs>().spa == ideal.evs.spa);
  REQUIRE(truth.get<EVs>().spd == ideal.evs.spd);
  REQUIRE(truth.get<EVs>().spe == ideal.evs.spe);

  REQUIRE(truth.get<IVs>().hp == ideal.ivs.hp);
  REQUIRE(truth.get<IVs>().atk == ideal.ivs.atk);
  REQUIRE(truth.get<IVs>().def == ideal.ivs.def);
  REQUIRE(truth.get<IVs>().spa == ideal.ivs.spa);
  REQUIRE(truth.get<IVs>().spd == ideal.ivs.spd);
  REQUIRE(truth.get<IVs>().spe == ideal.ivs.spe);

  const std::vector<entt::entity>& moveSlotsTruth = truth.get<MoveSlots>().moveSlots;
  REQUIRE(moveSlotsTruth.size() == ideal.moves.size());

  for (std::size_t i = 0; i < ideal.moves.size(); i++) {
    const Simulation::MoveCreationInfo& moveIdeal = ideal.moves[i];
    entt::entity moveTruth = moveSlotsTruth[i];
    REQUIRE(truth.registry()->all_of<MoveName, PP, MaxPP>(moveTruth));
    REQUIRE(truth.registry()->get<MoveName>(moveTruth).name == moveIdeal.name);
    REQUIRE(truth.registry()->get<PP>(moveTruth).pp == moveIdeal.pp);
    REQUIRE(truth.registry()->get<MaxPP>(moveTruth).maxPP == moveIdeal.maxPP);
  }
}

void checkCreatedSide(const entt::handle& truth, const Simulation::SideCreationInfo& ideal) {
  REQUIRE(truth.all_of<Team, FoeSide, Battle>());

  const std::vector<entt::entity>& teamTruth = truth.get<Team>().team;
  REQUIRE(teamTruth.size() == ideal.team.size());

  for (std::size_t i = 0; i < ideal.team.size(); i++) {
    entt::entity pokemonTruth = teamTruth[i];
    entt::handle pokemonIdeal(*truth.registry(), pokemonTruth);
    checkCreatedPokemon(pokemonIdeal, ideal.team[i]);

    REQUIRE(pokemonIdeal.get<Side>().side == truth.entity());
    REQUIRE(pokemonIdeal.get<Battle>().battle == truth.get<Battle>().battle);
  }
}

void checkCreatedBattle(const entt::handle truth, const Simulation::BattleCreationInfo& ideal) {
  REQUIRE(truth.any_of<Sides, ActionQueue, Turn, Probability, RNGSeed>());
  if (ideal.turn) REQUIRE(truth.get<Turn>().turn == ideal.turn);
  else REQUIRE(truth.get<Turn>().turn == 0);

  if (ideal.probability) REQUIRE(truth.get<Probability>().probability == ideal.probability);
  else REQUIRE(truth.get<Probability>().probability == 1);

  if (ideal.rngSeed) REQUIRE(truth.get<RNGSeed>().seed == ideal.rngSeed);
  else REQUIRE(truth.get<RNGSeed>().seed != 0);

  auto [p1SideEntity, p2SideEntity] = truth.get<Sides>();
  entt::handle p1SideTruth(*truth.registry(), p1SideEntity);
  entt::handle p2SideTruth(*truth.registry(), p2SideEntity);

  checkCreatedSide(p1SideTruth, ideal.P1);
  checkCreatedSide(p2SideTruth, ideal.P2);

  REQUIRE(p1SideTruth.get<Battle>().battle == truth.entity());
  REQUIRE(p2SideTruth.get<Battle>().battle == truth.entity());
  REQUIRE(p1SideTruth.get<FoeSide>().foeSide == p2SideEntity);
  REQUIRE(p2SideTruth.get<FoeSide>().foeSide == p1SideEntity);
}

TEST_CASE("Single Battle", "[BattleState]") {
  Pokedex pokedex(SCARLET_VIOLET_GAME_MECHANICS);
  entt::dense_set<dex::Move> moveSet{};
  for (dex::Move move : {dex::FURY_ATTACK, dex::THUNDERBOLT}) moveSet.insert(move);

  pokedex.loadMoves(moveSet);

  Simulation simulation(pokedex, SINGLES_BATTLE_FORMAT);

  Simulation::BattleCreationInfo battleCreationInfo{};
  {
    Simulation::PokemonCreationInfo p1PokemonInfo{};
    p1PokemonInfo.species = dex::EMPOLEON;
    p1PokemonInfo.ability = dex::DEFIANT;
    p1PokemonInfo.gender = dex::MALE;
    p1PokemonInfo.status = dex::PAR;
    p1PokemonInfo.level = 99;
    p1PokemonInfo.evs = {0, 50, 100, 150, 200, 255};
    p1PokemonInfo.stats = {309, 208, 212, 258, 238, 156};

    Simulation::MoveCreationInfo p1MoveInfo{};
    p1MoveInfo.name = dex::FURY_ATTACK;
    p1MoveInfo.maxPP = pokedex.getMoveData<PP>(dex::FURY_ATTACK).pp;
    p1MoveInfo.pp =  p1MoveInfo.maxPP - 1;
    p1PokemonInfo.moves.push_back(std::move(p1MoveInfo));

    Simulation::PokemonCreationInfo p2PokemonInfo{};
    p2PokemonInfo.species = dex::AMPHAROS;
    p2PokemonInfo.item = dex::CHOICE_SPECS;
    p2PokemonInfo.ability = dex::STATIC;
    p2PokemonInfo.gender = dex::FEMALE;
    p2PokemonInfo.level = 100;
    p2PokemonInfo.nature = dex::MODEST;
    p2PokemonInfo.ivs = {5, 10, 15, 20, 25, 30};
    p2PokemonInfo.stats = {321, 186, 206, 266, 216, 146};

    Simulation::MoveCreationInfo p2MoveInfo{};
    p2MoveInfo.name = dex::THUNDERBOLT;
    p2MoveInfo.maxPP = pokedex.getMoveData<PP>(dex::THUNDERBOLT).pp;
    p2MoveInfo.pp = p2MoveInfo.maxPP - 2;
    p2PokemonInfo.moves.push_back(std::move(p2MoveInfo));

    battleCreationInfo.P1 = {{std::move(p1PokemonInfo)}};
    battleCreationInfo.P2 = {{std::move(p2PokemonInfo)}};
  }

  simulation.createInitialStates({battleCreationInfo});

  entt::registry& registry = simulation.registry;
  auto battles = registry.view<Sides>();
  REQUIRE(battles.size() == 1);
  entt::entity battleEntity = battles[0];

  checkCreatedBattle(entt::handle(registry, battleEntity), battleCreationInfo);

  auto [p1SideEntity, p2SideEntity] = registry.get<Sides>(battleEntity);

  entt::entity p1Entity = registry.get<Team>(p1SideEntity).team[0];
  REQUIRE(registry.all_of<tags::ability::Defiant>(p1Entity));
  REQUIRE(registry.all_of< tags::status::Paralysis>(p1Entity));
  entt::entity p2Entity = registry.get<Team>(p2SideEntity).team[0];
  REQUIRE(registry.all_of<tags::ability::Static>(p2Entity));
  REQUIRE(registry.all_of<tags::nature::Modest>(p2Entity));
  REQUIRE(registry.all_of<tags::item::ChoiceSpecs>(p2Entity));
}

TEST_CASE("Double Battle", "[BattleState]") {
  Pokedex pokedex(SCARLET_VIOLET_GAME_MECHANICS);
  entt::dense_set<dex::Move> moveSet{};
  for (dex::Move move : {dex::MOONBLAST, dex::KNOCK_OFF, dex::WILL_O_WISP, dex::QUIVER_DANCE}) moveSet.insert(move);

  pokedex.loadMoves(moveSet);

  Simulation simulation(pokedex, DOUBLES_BATTLE_FORMAT);

  Simulation::BattleCreationInfo battleCreationInfo{};
  {
    Simulation::PokemonCreationInfo p1aPokemonInfo{};
    p1aPokemonInfo.species = dex::GARDEVOIR;
    p1aPokemonInfo.ability = dex::TRACE;
    p1aPokemonInfo.gender = dex::FEMALE;
    p1aPokemonInfo.item = dex::CHOICE_SCARF;
    p1aPokemonInfo.status = dex::BRN;
    p1aPokemonInfo.level = 90;
    p1aPokemonInfo.stats = {277, 166, 166, 286, 266, 196};
    p1aPokemonInfo.evs = {0, 50, 100, 150, 200, 255};

    Simulation::MoveCreationInfo p1aMoveInfo{};
    p1aMoveInfo.name = dex::MOONBLAST;
    p1aMoveInfo.maxPP = pokedex.getMoveData<PP>(dex::MOONBLAST).pp;
    p1aMoveInfo.pp =  p1aMoveInfo.maxPP - 4;
    p1aPokemonInfo.moves.push_back(std::move(p1aMoveInfo));

    Simulation::PokemonCreationInfo p1bPokemonInfo{};
    p1bPokemonInfo.species = dex::PANGORO;
    p1bPokemonInfo.ability = dex::IRON_FIST;
    p1bPokemonInfo.gender = dex::MALE;
    p1bPokemonInfo.item = dex::LIFE_ORB;
    p1bPokemonInfo.level = 95;
    p1bPokemonInfo.stats = {331, 284, 192, 174, 178, 152};
    p1bPokemonInfo.evs = {255, 200, 150, 100, 50, 0};

    Simulation::MoveCreationInfo p1bMoveInfo{};
    p1bMoveInfo.name = dex::KNOCK_OFF;
    p1bMoveInfo.maxPP = pokedex.getMoveData<PP>(dex::KNOCK_OFF).pp;
    p1bMoveInfo.pp =  p1bMoveInfo.maxPP - 3;
    p1bPokemonInfo.moves.push_back(std::move(p1bMoveInfo));

    Simulation::PokemonCreationInfo p2aPokemonInfo{};
    p2aPokemonInfo.species = dex::DRAGAPULT;
    p2aPokemonInfo.ability = dex::INFILTRATOR;
    p2aPokemonInfo.gender = dex::FEMALE;
    p2aPokemonInfo.item = dex::FOCUS_SASH;
    p2aPokemonInfo.level = 100;
    p2aPokemonInfo.nature = dex::HASTY;
    p2aPokemonInfo.stats = {217, 276, 186, 236, 186, 320};
    p2aPokemonInfo.ivs = {5, 10, 15, 20, 25, 30};

    Simulation::MoveCreationInfo p2aMoveInfo{};
    p2aMoveInfo.name = dex::WILL_O_WISP;
    p2aMoveInfo.maxPP = pokedex.getMoveData<PP>(dex::WILL_O_WISP).pp;
    p2aMoveInfo.pp =  p2aMoveInfo.maxPP - 2;
    p2aPokemonInfo.moves.push_back(std::move(p2aMoveInfo));

    Simulation::PokemonCreationInfo p2bPokemonInfo{};
    p2bPokemonInfo.species = dex::RIBOMBEE;
    p2bPokemonInfo.ability = dex::SWEET_VEIL;
    p2bPokemonInfo.gender = dex::MALE;
    p2bPokemonInfo.item = dex::BRIGHT_POWDER;
    p2aPokemonInfo.level = 91;
    p2bPokemonInfo.stats = {261, 146, 156, 226, 176, 284};
    p2bPokemonInfo.ivs = {30, 25, 20, 15, 10, 5};

    Simulation::MoveCreationInfo p2bMoveInfo{};
    p2bMoveInfo.name = dex::QUIVER_DANCE;
    p2bMoveInfo.maxPP = pokedex.getMoveData<PP>(dex::QUIVER_DANCE).pp;
    p2bMoveInfo.pp =  p2bMoveInfo.maxPP - 1;
    p2bPokemonInfo.moves.push_back(std::move(p2bMoveInfo));

    battleCreationInfo.P1 = {{std::move(p1aPokemonInfo), std::move(p1bPokemonInfo)}};
    battleCreationInfo.P2 = {{std::move(p2aPokemonInfo), std::move(p2bPokemonInfo)}};
  }

  battleCreationInfo.turn = 2;
  battleCreationInfo.probability = 0.9;
  battleCreationInfo.rngSeed = 0x12345678;
  simulation.createInitialStates({battleCreationInfo});

  entt::registry& registry = simulation.registry;
  auto battles = registry.view<Sides>();
  REQUIRE(battles.size() == 1);
  entt::entity battleEntity = battles[0];

  checkCreatedBattle(entt::handle(registry, battleEntity), battleCreationInfo);

  auto [p1SideEntity, p2SideEntity] = registry.get<Sides>(battleEntity);

  entt::entity p1aEntity = registry.get<Team>(p1SideEntity).team[0];
  REQUIRE(registry.all_of<tags::ability::Trace>(p1aEntity));
  REQUIRE(registry.all_of<tags::status::Burn>(p1aEntity));
  REQUIRE(registry.all_of<tags::item::ChoiceScarf>(p1aEntity));
  entt::entity p1bEntity = registry.get<Team>(p1SideEntity).team[1];
  REQUIRE(registry.all_of<tags::ability::IronFist>(p1bEntity));
  REQUIRE(registry.all_of<tags::item::LifeOrb>(p1bEntity));

  entt::entity p2aEntity = registry.get<Team>(p2SideEntity).team[0];
  REQUIRE(registry.all_of<tags::ability::Infiltrator>(p2aEntity));
  REQUIRE(registry.all_of<tags::nature::Hasty>(p2aEntity));
  REQUIRE(registry.all_of<tags::item::FocusSash>(p2aEntity));
  entt::entity p2bEntity = registry.get<Team>(p2SideEntity).team[1];
  REQUIRE(registry.all_of<tags::ability::SweetVeil>(p2bEntity));
  REQUIRE(registry.all_of<tags::item::BrightPowder>(p2bEntity));
}
}  // namespace pokesim