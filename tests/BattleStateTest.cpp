#include "Tests.hpp"

namespace pokesim {
TEST_CASE("Single Battle", "[BattleState]") {
  Pokedex pokedex(SCARLET_VIOLET_GAME_MECHANICS);
  entt::dense_set<dex::Move> moveSet{};
  for (dex::Move move : {dex::FURY_ATTACK, dex::THUNDERBOLT}) moveSet.insert(move);

  pokedex.loadMoves(moveSet);

  uint8_t idealP1MoveMaxPP = pokedex.getMoveData<PP>(dex::FURY_ATTACK).pp;
  uint8_t idealP1MovePP = idealP1MoveMaxPP - 1;
  uint8_t idealP2MoveMaxPP = pokedex.getMoveData<PP>(dex::THUNDERBOLT).pp;
  uint8_t idealP2MovePP = idealP2MoveMaxPP - 2;

  Simulation simulation(pokedex, SINGLES_BATTLE_FORMAT);
  EVs idealP1Evs = {0, 50, 100, 150, 200, 255};
  IVs idealP2Ivs = {5, 10, 15, 20, 25, 30};

  Simulation::BattleCreationInfo battleCreationInfo{};
  {
    Simulation::PokemonCreationInfo p1PokemonInfo{};
    p1PokemonInfo.species = dex::EMPOLEON;
    p1PokemonInfo.ability = dex::DEFIANT;
    p1PokemonInfo.gender = dex::MALE;
    p1PokemonInfo.status = dex::PAR;
    p1PokemonInfo.level = 99;
    p1PokemonInfo.evs = idealP1Evs;
    p1PokemonInfo.stats = {309, 208, 212, 258, 238, 156};

    Simulation::MoveCreationInfo p1MoveInfo{};
    p1MoveInfo.name = dex::FURY_ATTACK;
    p1MoveInfo.pp = idealP1MovePP;
    p1MoveInfo.maxPP = idealP1MoveMaxPP;
    p1PokemonInfo.moves.push_back(std::move(p1MoveInfo));

    Simulation::PokemonCreationInfo p2PokemonInfo{};
    p2PokemonInfo.species = dex::AMPHAROS;
    p2PokemonInfo.item = dex::CHOICE_SPECS;
    p2PokemonInfo.ability = dex::STATIC;
    p2PokemonInfo.gender = dex::FEMALE;
    p2PokemonInfo.level = 100;
    p2PokemonInfo.nature = dex::MODEST;
    p2PokemonInfo.ivs = idealP2Ivs;
    p2PokemonInfo.stats = {321, 186, 206, 266, 216, 146};

    Simulation::MoveCreationInfo p2MoveInfo{};
    p2MoveInfo.name = dex::THUNDERBOLT;
    p2MoveInfo.pp = idealP2MovePP;
    p2MoveInfo.maxPP = idealP2MoveMaxPP;
    p2PokemonInfo.moves.push_back(std::move(p2MoveInfo));

    battleCreationInfo.P1 = {{std::move(p1PokemonInfo)}};
    battleCreationInfo.P2 = {{std::move(p2PokemonInfo)}};
  }

  simulation.createInitialStates({std::move(battleCreationInfo)});

  const entt::registry& registry = simulation.registry;
  auto battles = registry.view<Sides>();
  REQUIRE(battles.size() == 1);
  entt::entity battleEntity = battles[0];

  REQUIRE(registry.any_of<Sides, ActionQueue, Turn, Probability, RNGSeed>(battleEntity));
  REQUIRE(registry.get<Turn>(battleEntity).turn == 0);
  REQUIRE(registry.get<Probability>(battleEntity).probability == 1);
  REQUIRE(registry.get<RNGSeed>(battleEntity).seed != 0);

  auto [p1SideEntity, p2SideEntity] = registry.get<Sides>(battleEntity);
  REQUIRE(registry.all_of<Team, FoeSide, Battle>(p1SideEntity));
  REQUIRE(registry.all_of<Team, FoeSide, Battle>(p2SideEntity));
  REQUIRE(registry.get<Battle>(p1SideEntity).battle == battleEntity);
  REQUIRE(registry.get<Battle>(p2SideEntity).battle == battleEntity);
  REQUIRE(registry.get<FoeSide>(p1SideEntity).foeSide == p2SideEntity);
  REQUIRE(registry.get<FoeSide>(p2SideEntity).foeSide == p1SideEntity);

  const std::vector<entt::entity>& p1Team = registry.get<Team>(p1SideEntity).team;
  const std::vector<entt::entity>& p2Team = registry.get<Team>(p2SideEntity).team;

  REQUIRE(p1Team.size() == 1);
  REQUIRE(p2Team.size() == 1);

  entt::entity p1Entity = p1Team[0];
  REQUIRE(
    registry.all_of<ID, Side, Battle, SpeciesName, AbilityName, GenderName, Level, MoveSlots, EVs, IVs>(p1Entity));
  REQUIRE(registry.all_of<stat::HP, stat::Atk, stat::Def, stat::Spa, stat::Spd, stat::Spe>(p1Entity));
  REQUIRE(registry.all_of<AbilityName, StatusName, tags::ability::Defiant, tags::status::Paralysis>(p1Entity));
  REQUIRE_FALSE(registry.any_of<ItemName, NatureName>(p1Entity));

  REQUIRE(registry.get<SpeciesName>(p1Entity).name == dex::EMPOLEON);
  REQUIRE(registry.get<AbilityName>(p1Entity).name == dex::DEFIANT);
  REQUIRE(registry.get<GenderName>(p1Entity).name == dex::MALE);
  REQUIRE(registry.get<Level>(p1Entity).level == 99);
  REQUIRE(
    (registry.get<stat::HP>(p1Entity).stat == 309 && registry.get<stat::Atk>(p1Entity).stat == 208 &&
     registry.get<stat::Def>(p1Entity).stat == 212 && registry.get<stat::Spa>(p1Entity).stat == 258 &&
     registry.get<stat::Spd>(p1Entity).stat == 238 && registry.get<stat::Spe>(p1Entity).stat == 156));

  EVs trueP1Evs = registry.get<EVs>(p1Entity);
  REQUIRE(
    (trueP1Evs.hp == idealP1Evs.hp && trueP1Evs.atk == idealP1Evs.atk && trueP1Evs.def == idealP1Evs.def &&
     trueP1Evs.spa == idealP1Evs.spa && trueP1Evs.spd == idealP1Evs.spd && trueP1Evs.spd == idealP1Evs.spd));
  REQUIRE(registry.get<IVs>(p1Entity).atk == 0);

  const std::vector<entt::entity>& p1MoveSlots = registry.get<MoveSlots>(p1Entity).moveSlots;
  REQUIRE(p1MoveSlots.size() == 1);
  REQUIRE(registry.all_of<MoveName, PP, MaxPP>(p1MoveSlots[0]));
  REQUIRE(registry.get<MoveName>(p1MoveSlots[0]).name == dex::FURY_ATTACK);
  REQUIRE(registry.get<PP>(p1MoveSlots[0]).pp == idealP1MovePP);
  REQUIRE(registry.get<MaxPP>(p1MoveSlots[0]).maxPP == idealP1MoveMaxPP);

  entt::entity p2Entity = p2Team[0];
  REQUIRE(
    registry.all_of<ID, Side, Battle, SpeciesName, AbilityName, GenderName, Level, MoveSlots, EVs, IVs>(p2Entity));
  REQUIRE(registry.all_of<stat::HP, stat::Atk, stat::Def, stat::Spa, stat::Spd, stat::Spe>(p2Entity));
  REQUIRE(
    registry
      .all_of<AbilityName, NatureName, ItemName, tags::ability::Static, tags::nature::Modest, tags::item::ChoiceSpecs>(
        p2Entity));
  REQUIRE_FALSE(registry.any_of<StatusName>(p2Entity));

  REQUIRE(registry.get<SpeciesName>(p2Entity).name == dex::AMPHAROS);
  REQUIRE(registry.get<AbilityName>(p2Entity).name == dex::STATIC);
  REQUIRE(registry.get<ItemName>(p2Entity).name == dex::CHOICE_SPECS);
  REQUIRE(registry.get<GenderName>(p2Entity).name == dex::FEMALE);
  REQUIRE(registry.get<Level>(p2Entity).level == 100);
  REQUIRE(
    (registry.get<stat::HP>(p2Entity).stat == 321 && registry.get<stat::Atk>(p2Entity).stat == 186 &&
     registry.get<stat::Def>(p2Entity).stat == 206 && registry.get<stat::Spa>(p2Entity).stat == 266 &&
     registry.get<stat::Spd>(p2Entity).stat == 216 && registry.get<stat::Spe>(p2Entity).stat == 146));

  IVs trueP2Ivs = registry.get<IVs>(p2Entity);
  REQUIRE(
    (trueP2Ivs.hp == idealP2Ivs.hp && trueP2Ivs.atk == idealP2Ivs.atk && trueP2Ivs.def == idealP2Ivs.def &&
     trueP2Ivs.spa == idealP2Ivs.spa && trueP2Ivs.spd == idealP2Ivs.spd && trueP2Ivs.spd == idealP2Ivs.spd));
  REQUIRE(registry.get<EVs>(p2Entity).atk == 0);

  const std::vector<entt::entity>& p2MoveSlots = registry.get<MoveSlots>(p2Entity).moveSlots;
  REQUIRE(p2MoveSlots.size() == 1);
  REQUIRE(registry.all_of<MoveName, PP, MaxPP>(p2MoveSlots[0]));
  REQUIRE(registry.get<MoveName>(p2MoveSlots[0]).name == dex::THUNDERBOLT);
  REQUIRE(registry.get<PP>(p2MoveSlots[0]).pp == idealP2MovePP);
  REQUIRE(registry.get<MaxPP>(p2MoveSlots[0]).maxPP == idealP2MoveMaxPP);
}
}  // namespace pokesim