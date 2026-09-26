#include "../Tests.hpp"

namespace pokesim {
namespace {

template <typename P1ADecision, typename P2ADecision>
void checkSwitchOptions(
  TestSimulation& test, TestSimulation::BattleEntities& entities, bool p2AIsTrapped, P1ADecision p1ADecision,
  P2ADecision p2ADecision) {
  simulate_turn::setSideOptions(test.simulation);

  if (test.simulation.isBattleFormat(BattleFormat::SINGLES)) {
    SinglesSideOptions p1Options = test.registry().get<SinglesSideOptions>(entities.p1Side);
    SinglesSideOptions p2Options = test.registry().get<SinglesSideOptions>(entities.p2Side);
    REQUIRE_FALSE(p1Options.switches.val.empty());
    REQUIRE(p2Options.switches.val.empty() == p2AIsTrapped);

    test.applyDecision(entities.battle, test.turnDecision(p1ADecision, p2ADecision));
    test.simulation.removeFromEntities<SinglesSideOptions>();
  }
  else {
    test.checks.checkUsedMovePokemon(entities.p1B);
    test.checks.checkUsedMovePokemon(entities.p2B);

    DoublesSideOptions p1Options = test.registry().get<DoublesSideOptions>(entities.p1Side);
    DoublesSideOptions p2Options = test.registry().get<DoublesSideOptions>(entities.p2Side);
    REQUIRE_FALSE(p1Options.switches[0].val.empty());
    REQUIRE_FALSE(p1Options.switches[1].val.empty());
    REQUIRE(p2Options.switches[0].val.empty() == p2AIsTrapped);
    REQUIRE_FALSE(p2Options.switches[1].val.empty());

    test.applyDecision(
      entities.battle,
      test.turnDecision(p1ADecision, dex::Move::SPLASH, p2ADecision, dex::Move::SPLASH));
    test.simulation.removeFromEntities<DoublesSideOptions>();
  }
}
}  // namespace

TEST_CASE(
  "Trapped: prevent from switching out", "[Simulation][SimulateTurn][SingleBattle][Effect][Volatile][Trapped]") {
  BattleFormat battleFormat = GENERATE(BattleFormat::SINGLES, BattleFormat::DOUBLES);
  CAPTURE(battleFormat);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, battleFormat};
  BattleCreationInfo& battleInfo = test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::DECIDUEYE, dex::Move::SPIRIT_SHACKLE),
      test.pokemon(dex::Species::MAGNEZONE, dex::Ability::ANALYTIC, dex::Move::SPLASH),
      test.pokemon(dex::Species::BRAVIARY, dex::Ability::DEFIANT, dex::Move::SPLASH)),
    test.side(
      test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH),
      test.pokemon(dex::Species::AMPHAROS, dex::Move::SPLASH),
      test.pokemon(dex::Species::GARDEVOIR, dex::Move::SPLASH)));

  battleInfo.runWithSimulateTurn = true;
  test.simulateTurnOptions().setRandomChanceUpperLimit(Constants::Probability::MIN);
  if (test.simulation.isBattleFormat(BattleFormat::SINGLES)) {
    battleInfo.decisionsToSimulate.push_back(test.turnDecision(dex::Move::SPIRIT_SHACKLE, dex::Move::SPLASH));
  }
  else {
    battleInfo.decisionsToSimulate.push_back(
      test.turnDecision(dex::Move::SPIRIT_SHACKLE, dex::Move::SPLASH, dex::Move::SPLASH, dex::Move::SPLASH));
  }

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(entities.p1A);
  test.checks.checkUsedMovePokemon<stat::CurrentHp, tags::Trapped, Trapper>(entities.p2A);

  auto trapper = test.registry().get<Trapper>(entities.p2A);
  REQUIRE(trapper.val == entities.p1A);

  checkSwitchOptions(test, entities, true, Slot::P1C, Slot::P2C);

  REQUIRE_THROWS(test.simulateOneNonBranchingBattle());
}

TEST_CASE(
  "Trapped: do not prevent Pokemon immune to trapping from switching out",
  "[Simulation][SimulateTurn][SingleBattle][Effect][Volatile][Trapped]") {
  BattleFormat battleFormat = GENERATE(BattleFormat::SINGLES, BattleFormat::DOUBLES);
  CAPTURE(battleFormat);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, battleFormat};
  BattleCreationInfo& battleInfo = test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::DECIDUEYE, Level{10U}, dex::Move::SPIRIT_SHACKLE),
      test.pokemon(dex::Species::MAGNEZONE, dex::Ability::ANALYTIC, dex::Move::SPLASH),
      test.pokemon(dex::Species::BRAVIARY, dex::Ability::DEFIANT, dex::Move::SPLASH)),
    test.side(
      test.pokemon(dex::Species::DRAGAPULT, dex::Move::SPLASH),
      test.pokemon(dex::Species::AMPHAROS, dex::Move::SPLASH),
      test.pokemon(dex::Species::GARDEVOIR, dex::Move::SPLASH)));

  battleInfo.runWithSimulateTurn = true;
  if (test.simulation.isBattleFormat(BattleFormat::SINGLES)) {
    battleInfo.decisionsToSimulate.push_back(test.turnDecision(dex::Move::SPIRIT_SHACKLE, dex::Move::SPLASH));
  }
  else {
    battleInfo.decisionsToSimulate.push_back(
      test.turnDecision(dex::Move::SPIRIT_SHACKLE, dex::Move::SPLASH, dex::Move::SPLASH, dex::Move::SPLASH));
  }

  auto turn1Entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(turn1Entities.p1A);
  test.checks.checkUsedMovePokemon<stat::CurrentHp>(turn1Entities.p2A);

  checkSwitchOptions(test, turn1Entities, false, Slot::P1C, Slot::P2C);

  auto turn2Entities = test.simulateOneNonBranchingBattle({}, Tags<Team>{});

  test.checks.checkEntityForChanges<tags::ActivePokemon>(turn2Entities.p1A);
  test.checks.checkEntityForChanges<tags::ActivePokemon>(turn2Entities.p2A);

  REQUIRE(test.registry().get<SpeciesName>(turn2Entities.p1A).val == dex::Species::BRAVIARY);
  REQUIRE(test.registry().get<SpeciesName>(turn2Entities.p2A).val == dex::Species::GARDEVOIR);
}

TEST_CASE(
  "Trapped: free all trapped Pokemon if the user is no longer active",
  "[Simulation][SimulateTurn][SingleBattle][Effect][Volatile][Trapped]") {
  BattleFormat battleFormat = GENERATE(BattleFormat::SINGLES, BattleFormat::DOUBLES);
  CAPTURE(battleFormat);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, battleFormat};
  BattleCreationInfo& battleInfo = test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::DECIDUEYE, dex::Move::SPIRIT_SHACKLE),
      test.pokemon(dex::Species::MAGNEZONE, dex::Ability::ANALYTIC, dex::Move::SPLASH),
      test.pokemon(dex::Species::BRAVIARY, dex::Ability::DEFIANT, dex::Move::SPLASH)),
    test.side(
      test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH),
      test.pokemon(dex::Species::AMPHAROS, dex::Move::SPLASH),
      test.pokemon(dex::Species::GARDEVOIR, dex::Move::SPLASH)));

  battleInfo.runWithSimulateTurn = true;
  test.simulateTurnOptions().setRandomChanceUpperLimit(Constants::Probability::MIN);
  if (test.simulation.isBattleFormat(BattleFormat::SINGLES)) {
    battleInfo.decisionsToSimulate.push_back(test.turnDecision(dex::Move::SPIRIT_SHACKLE, dex::Move::SPLASH));
  }
  else {
    battleInfo.decisionsToSimulate.push_back(
      test.turnDecision(dex::Move::SPIRIT_SHACKLE, dex::Move::SPLASH, dex::Move::SPLASH, dex::Move::SPLASH));
  }

  auto entities = test.simulateOneNonBranchingBattle();
  checkSwitchOptions(test, entities, true, Slot::P1C, dex::Move::SPLASH);
  test.simulateOneNonBranchingBattle({}, Tags<Team>{});

  checkSwitchOptions(test, entities, false, Slot::P1C, Slot::P2C);
  auto turn3Entities = test.simulateOneNonBranchingBattle({}, Tags<Team>{});

  REQUIRE(test.registry().get<SpeciesName>(turn3Entities.p1A).val == dex::Species::DECIDUEYE);
  REQUIRE(test.registry().get<SpeciesName>(turn3Entities.p2A).val == dex::Species::GARDEVOIR);
}

}  // namespace pokesim
