#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("Reversal: Changes base power based on HP", "[Simulation][SimulateTurn][SingleBattle][Move][Reversal]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};

  SECTION("Check callback base power") {
    REQUIRE(test.dexValue<dex::Reversal::onSetBasePowerFromSourceHp>(1000U, 1000U) == 20U);
    REQUIRE(test.dexValue<dex::Reversal::onSetBasePowerFromSourceHp>(688U, 1000U) == 20U);
    REQUIRE(test.dexValue<dex::Reversal::onSetBasePowerFromSourceHp>(687U, 1000U) == 40U);
    REQUIRE(test.dexValue<dex::Reversal::onSetBasePowerFromSourceHp>(353U, 1000U) == 80U);
    REQUIRE(test.dexValue<dex::Reversal::onSetBasePowerFromSourceHp>(207U, 1000U) == 100U);
    REQUIRE(test.dexValue<dex::Reversal::onSetBasePowerFromSourceHp>(103U, 1000U) == 150U);
    REQUIRE(test.dexValue<dex::Reversal::onSetBasePowerFromSourceHp>(41U, 1000U) == 200U);
    REQUIRE(test.dexValue<dex::Reversal::onSetBasePowerFromSourceHp>(1U, 1000U) == 200U);
  }

  SECTION("Check damage") {
    types::stat p1Hp = GENERATE(300U, 250U, 200U, 150U, 100U, 50U);
    types::stat idealP2Hp = 246U;
    switch (p1Hp) {
      case 300:
      case 250: idealP2Hp -= 12U; break;
      case 200:
      case 150: idealP2Hp -= 24U; break;
      case 100: idealP2Hp -= 47U; break;
      case 50:  idealP2Hp -= 59U; break;
      default:  FAIL();
    }
    CAPTURE(p1Hp, idealP2Hp);

    test.setupBattle(
      Turn{1U},
      test.side(test.pokemon(dex::Species::PANGORO, dex::Move::REVERSAL, stat::CurrentHp{p1Hp})),
      test.side(test.pokemon(dex::Species::GARDEVOIR, dex::Move::SPLASH)),
      test.turnDecision(dex::Move::REVERSAL, dex::Move::SPLASH));

    auto entities = test.simulateOneNonBranchingBattle();

    test.checks.checkUsedMovePokemon(entities.p1A);
    test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);

    stat::CurrentHp currentP2Hp = test.registry().get<stat::CurrentHp>(entities.p2A);
    REQUIRE(idealP2Hp == currentP2Hp.val);
  }
};

TEST_CASE(
  "Reversal: Changes damage in CalculateDamage", "[Simulation][CalculateDamage][SingleBattle][Move][Reversal]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  types::stat p1Hp = GENERATE(300U, 250U, 200U, 150U, 100U, 50U);
  types::damage idealDamage = 0U;
  switch (p1Hp) {
    case 300:
    case 250: idealDamage = 12U; break;
    case 200:
    case 150: idealDamage = 24U; break;
    case 100: idealDamage = 47U; break;
    case 50:  idealDamage = 59U; break;
    default:  FAIL();
  }
  CAPTURE(p1Hp, idealDamage);

  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::PANGORO, dex::Move::REVERSAL, stat::CurrentHp{p1Hp})),
    test.side(test.pokemon(dex::Species::GARDEVOIR, dex::Move::SPLASH)),
    CalcDamageInputInfo{Slot::P1A, Slot::P2A, {dex::Move::REVERSAL}});

  auto results = test.calculateDamage();
  results.damageRollResults().each(
    [&idealDamage](const DamageRolls& damageRolls) { REQUIRE(damageRolls.max() == idealDamage); });
}
}  // namespace pokesim
