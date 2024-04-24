#include "Tests.hpp"

namespace pokesim {
struct IdealPP_MaxPP {
  types::pp pp;
  types::pp maxPp;
};

void checkCreatedPokemon(const types::handle& truth, const Simulation::PokemonCreationInfo& ideal) {
  REQUIRE(truth.all_of<Id, Side, Battle, SpeciesName, AbilityName, Level, MoveSlots, Evs, Ivs>());
  REQUIRE(truth.all_of<stat::Hp, stat::Atk, stat::Def, stat::Spa, stat::Spd, stat::Spe>());

  if (ideal.id) REQUIRE(truth.get<Id>().id == ideal.id);
  else REQUIRE(truth.get<Id>().id != 0);

  REQUIRE(truth.get<SpeciesName>().name == ideal.species);
  REQUIRE(truth.get<AbilityName>().name == ideal.ability);
  REQUIRE(truth.get<Level>().level == ideal.level);

  if (ideal.item == dex::Item::NO_ITEM) REQUIRE_FALSE(truth.any_of<ItemName>());
  else REQUIRE(truth.get<ItemName>().name == ideal.item);

  if (ideal.gender == dex::Gender::NO_GENDER) REQUIRE_FALSE(truth.any_of<GenderName>());
  else REQUIRE(truth.get<GenderName>().name == ideal.gender);

  if (ideal.status == dex::Status::NO_STATUS) REQUIRE_FALSE(truth.any_of<StatusName>());
  else REQUIRE(truth.get<StatusName>().name == ideal.status);

  if (ideal.nature == dex::Nature::NO_NATURE) REQUIRE_FALSE(truth.any_of<NatureName>());
  else REQUIRE(truth.get<NatureName>().name == ideal.nature);

  REQUIRE(truth.get<stat::Hp>().stat == ideal.stats.hp);
  REQUIRE(truth.get<stat::Atk>().stat == ideal.stats.atk);
  REQUIRE(truth.get<stat::Def>().stat == ideal.stats.def);
  REQUIRE(truth.get<stat::Spa>().stat == ideal.stats.spa);
  REQUIRE(truth.get<stat::Spd>().stat == ideal.stats.spd);
  REQUIRE(truth.get<stat::Spe>().stat == ideal.stats.spe);

  REQUIRE(truth.get<Evs>().hp == ideal.evs.hp);
  REQUIRE(truth.get<Evs>().atk == ideal.evs.atk);
  REQUIRE(truth.get<Evs>().def == ideal.evs.def);
  REQUIRE(truth.get<Evs>().spa == ideal.evs.spa);
  REQUIRE(truth.get<Evs>().spd == ideal.evs.spd);
  REQUIRE(truth.get<Evs>().spe == ideal.evs.spe);

  REQUIRE(truth.get<Ivs>().hp == ideal.ivs.hp);
  REQUIRE(truth.get<Ivs>().atk == ideal.ivs.atk);
  REQUIRE(truth.get<Ivs>().def == ideal.ivs.def);
  REQUIRE(truth.get<Ivs>().spa == ideal.ivs.spa);
  REQUIRE(truth.get<Ivs>().spd == ideal.ivs.spd);
  REQUIRE(truth.get<Ivs>().spe == ideal.ivs.spe);

  const auto& moveSlotsTruth = truth.get<MoveSlots>().moveSlots;
  REQUIRE(moveSlotsTruth.size() == ideal.moves.size());

  for (std::size_t i = 0; i < ideal.moves.size(); i++) {
    const Simulation::MoveCreationInfo& moveIdeal = ideal.moves[i];
    types::entity moveTruth = moveSlotsTruth[(std::uint8_t)i];
    REQUIRE(truth.registry()->all_of<MoveName, Pp, MaxPp>(moveTruth));
    REQUIRE(truth.registry()->get<MoveName>(moveTruth).name == moveIdeal.name);
    REQUIRE(truth.registry()->get<Pp>(moveTruth).pp == moveIdeal.pp);
    REQUIRE(truth.registry()->get<MaxPp>(moveTruth).maxPp == moveIdeal.maxPp);
  }
}

void checkCreatedSide(const types::handle& truth, const Simulation::SideCreationInfo& ideal) {
  REQUIRE(truth.all_of<Team, FoeSide, Battle>());

  const auto& teamTruth = truth.get<Team>().team;
  REQUIRE(teamTruth.size() == ideal.team.size());

  for (std::size_t i = 0; i < ideal.team.size(); i++) {
    types::entity pokemonTruth = teamTruth[(std::uint8_t)i];
    types::handle pokemonIdeal(*truth.registry(), pokemonTruth);
    checkCreatedPokemon(pokemonIdeal, ideal.team[i]);

    REQUIRE(pokemonIdeal.get<Side>().side == truth.entity());
    REQUIRE(pokemonIdeal.get<Battle>().battle == truth.get<Battle>().battle);
  }
}

void checkCreatedBattle(const types::handle truth, const Simulation::BattleCreationInfo& ideal) {
  REQUIRE(truth.any_of<Sides, ActionQueue, Turn, Probability, RngSeed>());
  if (ideal.turn) REQUIRE(truth.get<Turn>().turn == ideal.turn);
  else REQUIRE(truth.get<Turn>().turn == 0);

  if (ideal.probability) REQUIRE(truth.get<Probability>().probability == ideal.probability);
  else REQUIRE(truth.get<Probability>().probability == 1);

  if (ideal.rngSeed) REQUIRE(truth.get<RngSeed>().seed == ideal.rngSeed);
  else REQUIRE(truth.get<RngSeed>().seed != 0);

  auto [p1SideEntity, p2SideEntity] = truth.get<Sides>();
  types::handle p1SideTruth(*truth.registry(), p1SideEntity);
  types::handle p2SideTruth(*truth.registry(), p2SideEntity);

  checkCreatedSide(p1SideTruth, ideal.p1);
  checkCreatedSide(p2SideTruth, ideal.p2);

  REQUIRE(p1SideTruth.get<Battle>().battle == truth.entity());
  REQUIRE(p2SideTruth.get<Battle>().battle == truth.entity());
  REQUIRE(p1SideTruth.get<FoeSide>().foeSide == p2SideEntity);
  REQUIRE(p2SideTruth.get<FoeSide>().foeSide == p1SideEntity);
}

TEST_CASE("Battle State: Single Battle", "[BattleState][Setup]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  Simulation simulation = createSingleBattleSimulation(battleCreationInfo);

  simulation.createInitialStates({battleCreationInfo});

  types::registry& registry = simulation.registry;
  auto battles = registry.view<Sides>();
  REQUIRE(battles.size() == 1);
  types::entity battleEntity = battles[0];

  checkCreatedBattle(types::handle(registry, battleEntity), battleCreationInfo);

  auto [p1SideEntity, p2SideEntity] = registry.get<Sides>(battleEntity);

  types::entity p1Entity = registry.get<Team>(p1SideEntity).team[0];
  REQUIRE(registry.all_of<ability::tags::Defiant>(p1Entity));
  REQUIRE(registry.all_of<status::tags::Paralysis>(p1Entity));
  types::entity p2Entity = registry.get<Team>(p2SideEntity).team[0];
  REQUIRE(registry.all_of<ability::tags::Static>(p2Entity));
  REQUIRE(registry.all_of<nature::tags::Modest>(p2Entity));
  REQUIRE(registry.all_of<item::tags::ChoiceSpecs>(p2Entity));
}

TEST_CASE("Battle State: Double Battle", "[BattleState][Setup]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  Simulation simulation = createDoubleBattleSimulation(battleCreationInfo);
  simulation.createInitialStates({battleCreationInfo});

  types::registry& registry = simulation.registry;
  auto battles = registry.view<Sides>();
  REQUIRE(battles.size() == 1);
  types::entity battleEntity = battles[0];

  checkCreatedBattle(types::handle(registry, battleEntity), battleCreationInfo);

  auto [p1SideEntity, p2SideEntity] = registry.get<Sides>(battleEntity);

  types::entity p1aEntity = registry.get<Team>(p1SideEntity).team[0];
  REQUIRE(registry.all_of<ability::tags::Trace>(p1aEntity));
  REQUIRE(registry.all_of<status::tags::Burn>(p1aEntity));
  REQUIRE(registry.all_of<item::tags::ChoiceScarf>(p1aEntity));
  types::entity p1bEntity = registry.get<Team>(p1SideEntity).team[1];
  REQUIRE(registry.all_of<ability::tags::IronFist>(p1bEntity));
  REQUIRE(registry.all_of<item::tags::LifeOrb>(p1bEntity));

  types::entity p2aEntity = registry.get<Team>(p2SideEntity).team[0];
  REQUIRE(registry.all_of<ability::tags::Infiltrator>(p2aEntity));
  REQUIRE(registry.all_of<nature::tags::Hasty>(p2aEntity));
  REQUIRE(registry.all_of<item::tags::FocusSash>(p2aEntity));
  types::entity p2bEntity = registry.get<Team>(p2SideEntity).team[1];
  REQUIRE(registry.all_of<ability::tags::SweetVeil>(p2bEntity));
  REQUIRE(registry.all_of<item::tags::BrightPowder>(p2bEntity));
}

TEST_CASE("Battle State: Multiple Battles", "[BattleState][Setup]") {
  Simulation::BattleCreationInfo battle1CreationInfo{};
  Simulation simulation = createSingleBattleSimulation(battle1CreationInfo);

  Simulation::BattleCreationInfo battle2CreationInfo = battle1CreationInfo;

  battle1CreationInfo.turn = 12;
  battle1CreationInfo.probability = 0.1F;
  battle1CreationInfo.rngSeed = 0xFFFFFFFF;

  battle2CreationInfo.turn = 7;
  battle2CreationInfo.probability = 1.0F;
  battle2CreationInfo.rngSeed = 9826;
  simulation.createInitialStates({battle1CreationInfo, battle2CreationInfo});

  types::registry& registry = simulation.registry;
  auto battles = registry.view<Sides>();
  REQUIRE(battles.size() == 2);

  types::handle battleA = types::handle(registry, battles[0]);
  types::handle battleB = types::handle(registry, battles[1]);

  if (battleA.get<Turn>().turn == battle1CreationInfo.turn) {
    checkCreatedBattle(battleA, battle1CreationInfo);
    checkCreatedBattle(battleB, battle2CreationInfo);
  }
  else {
    checkCreatedBattle(battleA, battle2CreationInfo);
    checkCreatedBattle(battleB, battle1CreationInfo);
  }
}

TEST_CASE("Clone Battles", "[BattleState][Setup]") {
  Simulation::BattleCreationInfo battleCreationInfo{};

  auto check = [&](decltype(createSingleBattleSimulation) create, types::cloneIndex cloneCount) {
    INFO("Clone Count: " + std::to_string(cloneCount));

    Simulation simulation = create(battleCreationInfo);
    simulation.createInitialStates({battleCreationInfo});

    types::registry& registry = simulation.registry;
    types::handle baseHandle(registry, registry.view<Sides>()[0]);
    baseHandle.emplace<tags::CloneFrom>();

    clone(registry, cloneCount);

    auto battles = registry.view<Sides>();
    REQUIRE(battles.size() == cloneCount + 1);
    REQUIRE(registry.view<tags::CloneFrom>().empty());
    REQUIRE(registry.view<CloneTo>().empty());

    auto baseSides = baseHandle.get<Sides>();

    for (auto battle : battles) {
      types::handle cloneHandle(registry, battle);
      checkCreatedBattle(cloneHandle, battleCreationInfo);

      if (battle == baseHandle.entity()) continue;

      auto cloneSides = cloneHandle.get<Sides>();
      REQUIRE(baseSides.p1 != cloneSides.p1);
      REQUIRE(baseSides.p2 != cloneSides.p2);

      auto checkSideEntities = [&](types::entity baseSide, types::entity cloneSide) {
        const auto& [baseTeam, baseFoeSide, baseSideBattle] = registry.get<Team, FoeSide, Battle>(baseSide);
        const auto& [cloneTeam, cloneFoeSide, cloneSideBattle] = registry.get<Team, FoeSide, Battle>(cloneSide);
        REQUIRE(baseFoeSide.foeSide != cloneFoeSide.foeSide);
        REQUIRE(baseSideBattle.battle != cloneSideBattle.battle);

        for (std::uint8_t i = 0; i < baseTeam.team.size(); i++) {
          REQUIRE(baseTeam.team[i] != cloneTeam.team[i]);
          const auto& [baseMoveSlots, basePokemonSide, basePokemonBattle] =
            registry.get<MoveSlots, Side, Battle>(baseTeam.team[i]);
          const auto& [cloneMoveSlots, clonePokemonSide, clonePokemonBattle] =
            registry.get<MoveSlots, Side, Battle>(cloneTeam.team[i]);

          REQUIRE(basePokemonSide.side != clonePokemonSide.side);
          REQUIRE(basePokemonBattle.battle != clonePokemonBattle.battle);

          for (std::uint8_t j = 0; j < baseMoveSlots.moveSlots.size(); j++) {
            REQUIRE(baseMoveSlots.moveSlots[j] != cloneMoveSlots.moveSlots[j]);
          }
        }
      };

      checkSideEntities(baseSides.p1, cloneSides.p1);
      checkSideEntities(baseSides.p2, cloneSides.p2);
    }
  };

  SECTION("Single Battles") {
    check(createSingleBattleSimulation, 1);
    check(createSingleBattleSimulation, 5);
  }

  SECTION("Double Battles") {
    check(createDoubleBattleSimulation, 1);
    check(createDoubleBattleSimulation, 5);
  }
}
}  // namespace pokesim