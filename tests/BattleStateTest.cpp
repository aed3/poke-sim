#include "Tests.hpp"

namespace pokesim {
struct IdealPP_MaxPP {
  types::pp pp;
  types::pp maxPp;
};

TEST_CASE("Battle State: Single Battle", "[BattleState][Setup]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  Simulation simulation = createSingleBattleSimulation(battleCreationInfo, false);

  simulation.createInitialStates({battleCreationInfo});

  const types::registry& registry = simulation.registry;
  auto battles = registry.view<tags::Battle>();
  REQUIRE(battles.size() == 1);
  types::entity battleEntity = battles[0];

  auto [p1SideEntity, p2SideEntity] = registry.get<Sides>(battleEntity).val;

  types::entity p1Entity = registry.get<Team>(p1SideEntity).val[0];
  REQUIRE(registry.all_of<ability::tags::Defiant>(p1Entity));
  REQUIRE(registry.all_of<status::tags::Paralysis>(p1Entity));
  types::entity p2Entity = registry.get<Team>(p2SideEntity).val[0];
  REQUIRE(registry.all_of<ability::tags::Static>(p2Entity));
  REQUIRE(registry.all_of<nature::tags::Modest>(p2Entity));
  REQUIRE(registry.all_of<item::tags::ChoiceSpecs>(p2Entity));
}

TEST_CASE("Battle State: Double Battle", "[BattleState][Setup]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  Simulation simulation = createDoubleBattleSimulation(battleCreationInfo, false);
  simulation.createInitialStates({battleCreationInfo});

  const types::registry& registry = simulation.registry;
  auto battles = registry.view<tags::Battle>();
  REQUIRE(battles.size() == 1);
  types::entity battleEntity = battles[0];

  auto [p1SideEntity, p2SideEntity] = registry.get<Sides>(battleEntity).val;

  types::entity p1aEntity = registry.get<Team>(p1SideEntity).val[0];
  REQUIRE(registry.all_of<ability::tags::Trace>(p1aEntity));
  REQUIRE(registry.all_of<status::tags::Burn>(p1aEntity));
  REQUIRE(registry.all_of<item::tags::ChoiceScarf>(p1aEntity));
  types::entity p2aEntity = registry.get<Team>(p2SideEntity).val[0];
  REQUIRE(registry.all_of<ability::tags::IronFist>(p2aEntity));
  REQUIRE(registry.all_of<item::tags::LifeOrb>(p2aEntity));

  types::entity p1bEntity = registry.get<Team>(p1SideEntity).val[1];
  REQUIRE(registry.all_of<ability::tags::Infiltrator>(p1bEntity));
  REQUIRE(registry.all_of<nature::tags::Hasty>(p1bEntity));
  REQUIRE(registry.all_of<item::tags::FocusSash>(p1bEntity));
  types::entity p2bEntity = registry.get<Team>(p2SideEntity).val[1];
  REQUIRE(registry.all_of<ability::tags::SweetVeil>(p2bEntity));
  REQUIRE(registry.all_of<item::tags::BrightPowder>(p2bEntity));
}

TEST_CASE("Battle State: Multiple Battles", "[BattleState][Setup]") {
  Simulation::BattleCreationInfo battle1CreationInfo{};
  Simulation simulation = createSingleBattleSimulation(battle1CreationInfo, false);

  Simulation::BattleCreationInfo battle2CreationInfo = battle1CreationInfo;

  battle1CreationInfo.turn = 12;
  battle1CreationInfo.probability = 0.1F;
  battle1CreationInfo.rngSeed = 0xFFFFFFFF;

  battle2CreationInfo.turn = 7;
  battle2CreationInfo.probability = 1.0F;
  battle2CreationInfo.rngSeed = 9826;
  simulation.createInitialStates({battle1CreationInfo, battle2CreationInfo});

  const types::registry& registry = simulation.registry;
  auto battles = registry.view<tags::Battle>();
  REQUIRE(battles.size() == 2);
}

TEST_CASE("Clone Battles", "[BattleState][Setup]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  auto create = GENERATE(createSingleBattleSimulation, createDoubleBattleSimulation);
  types::cloneIndex cloneCount = GENERATE(1, 5, 100);

  CAPTURE(cloneCount);
  INFO(
    "create := " +
    std::string(
      create == createSingleBattleSimulation ? "createSingleBattleSimulation" : "createDoubleBattleSimulation"));

  Simulation simulation = create(battleCreationInfo, false);
  simulation.createInitialStates({battleCreationInfo});

  types::registry& registry = simulation.registry;
  types::handle baseHandle(registry, registry.view<tags::Battle>()[0]);
  baseHandle.emplace<tags::CloneFrom>();

  clone(registry, cloneCount);

  const auto battles = registry.view<tags::Battle>();
  REQUIRE(battles.size() == cloneCount + 1);
  REQUIRE(registry.view<tags::CloneFrom>().empty());
  REQUIRE(registry.view<CloneTo>().empty());

  const auto baseSides = baseHandle.get<Sides>();

  for (auto battle : battles) {
    const types::handle cloneHandle(registry, battle);
    debug::SimulationSetupChecks::checkBattle(registry, battle, battleCreationInfo);

    if (battle == baseHandle.entity()) continue;

    const auto cloneSides = cloneHandle.get<Sides>();
    REQUIRE(baseSides.p1() != cloneSides.p1());
    REQUIRE(baseSides.p2() != cloneSides.p2());

    auto checkSideEntities = [&](types::entity baseSide, types::entity cloneSide) {
      const auto& [baseTeam, baseFoeSide, baseSideBattle] = registry.get<Team, FoeSide, Battle>(baseSide);
      const auto& [cloneTeam, cloneFoeSide, cloneSideBattle] = registry.get<Team, FoeSide, Battle>(cloneSide);
      REQUIRE(baseFoeSide.val != cloneFoeSide.val);
      REQUIRE(baseSideBattle.val != cloneSideBattle.val);

      for (std::uint8_t i = 0; i < baseTeam.val.size(); i++) {
        REQUIRE(baseTeam.val[i] != cloneTeam.val[i]);
        const auto& [baseMoveSlots, basePokemonSide, basePokemonBattle] =
          registry.get<MoveSlots, Side, Battle>(baseTeam.val[i]);
        const auto& [cloneMoveSlots, clonePokemonSide, clonePokemonBattle] =
          registry.get<MoveSlots, Side, Battle>(cloneTeam.val[i]);

        REQUIRE(basePokemonSide.val != clonePokemonSide.val);
        REQUIRE(basePokemonBattle.val != clonePokemonBattle.val);

        for (std::uint8_t j = 0; j < baseMoveSlots.val.size(); j++) {
          REQUIRE(baseMoveSlots.val[j] != cloneMoveSlots.val[j]);
        }
      }
    };

    checkSideEntities(baseSides.p1(), cloneSides.p1());
    checkSideEntities(baseSides.p2(), cloneSides.p2());
  }
}
}  // namespace pokesim