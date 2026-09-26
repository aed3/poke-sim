#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("Analytic: Only boosts power when moving last", "[Simulation][SimulateTurn][Ability][Analytic]") {
  BattleFormat battleFormat = GENERATE(BattleFormat::SINGLES, BattleFormat::DOUBLES);
  bool shouldAnalyticBoost = GENERATE(true, false);
  CAPTURE(battleFormat, shouldAnalyticBoost);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, battleFormat};
  BattleCreationInfo& battleInfo = test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::MAGNEZONE, dex::Ability::ANALYTIC, dex::Move::TACKLE),
      test.pokemon(dex::Species::BRAVIARY, dex::Ability::DEFIANT, dex::Move::SPLASH)),
    test.side(
      test.pokemon(
        dex::Species::AMPHAROS,
        dex::Ability::PLUS,
        dex::Move::SPLASH,
        shouldAnalyticBoost ? dex::Nature::TIMID : dex::Nature::NO_NATURE),
      test.pokemon(dex::Species::GARDEVOIR, dex::Move::SPLASH)));

  battleInfo.runWithSimulateTurn = true;
  test.simulateTurnOptions().setRandomChanceUpperLimit(Constants::Probability::MIN);
  if (test.simulation.isBattleFormat(BattleFormat::SINGLES)) {
    battleInfo.decisionsToSimulate.push_back(test.turnDecision(dex::Move::TACKLE, dex::Move::SPLASH));
  }
  else {
    battleInfo.decisionsToSimulate.push_back(test.turnDecision(
      MoveDecision{Slot::P1A, Slot::P2B, dex::Move::TACKLE},
      dex::Move::SPLASH,
      dex::Move::SPLASH,
      dex::Move::SPLASH));
  }

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(entities.p1A);

  types::damage boostedDamage, unboostedDamage;
  types::entity defender;

  if (test.simulation.isBattleFormat(BattleFormat::SINGLES)) {
    test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);
    test.checks.checkEntityForChanges(entities.p1B);
    test.checks.checkEntityForChanges(entities.p2B);

    boostedDamage = 38U;
    unboostedDamage = 29U;
    defender = entities.p2A;
  }
  else {
    test.checks.checkUsedMovePokemon(entities.p2A);
    test.checks.checkUsedMovePokemon(entities.p1B);
    test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2B);

    boostedDamage = 48U;
    unboostedDamage = 38U;
    defender = entities.p2B;
  }

  auto [p2InitialHp, p2CurrentHp] = test.checks.getInitialAndCurrent<stat::CurrentHp>(defender);

  if (shouldAnalyticBoost) {
    REQUIRE(p2CurrentHp.val == p2InitialHp.val - boostedDamage);
  }
  else {
    REQUIRE(p2CurrentHp.val == p2InitialHp.val - unboostedDamage);
  }
}

TEST_CASE(
  "Analytic: Only boosts power when not faster than defender in CalculateDamage",
  "[Simulation][CalculateDamage][SingleBattle][Ability][Analytic]") {
  enum class P2SpeedDiff : std::uint8_t {
    SLOWER,
    SAME,
    FASTER,
  };

  P2SpeedDiff p2SpeedDiff = GENERATE(P2SpeedDiff::SLOWER, P2SpeedDiff::SAME, P2SpeedDiff::FASTER);

  dex::Nature p2Nature = dex::Nature::NO_NATURE;
  switch (p2SpeedDiff) {
    case P2SpeedDiff::SLOWER: p2Nature = dex::Nature::QUIET; break;
    case P2SpeedDiff::SAME:   p2Nature = dex::Nature::SERIOUS; break;
    case P2SpeedDiff::FASTER: p2Nature = dex::Nature::TIMID; break;
  }
  CAPTURE(p2SpeedDiff, p2Nature);

  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::MAGNEZONE, dex::Ability::ANALYTIC, dex::Move::TACKLE)),
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH, p2Nature)),
    CalcDamageInputInfo{Slot::P1A, Slot::P2A, {dex::Move::TACKLE}});

  auto result = test.calculateDamage();
  result.damageRollResults().each([&](const DamageRolls& damageRolls) {
    switch (p2SpeedDiff) {
      case P2SpeedDiff::FASTER:
      case P2SpeedDiff::SAME:   {
        REQUIRE(damageRolls.max() == 18U);
        break;
      }
      case P2SpeedDiff::SLOWER: {
        REQUIRE(damageRolls.max() == 14U);
        break;
      }
    }
  });
}
}  // namespace pokesim
