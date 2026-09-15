#include "../Tests.hpp"

namespace pokesim {
TEST_CASE(
  "Rocky Helmet: Hurt attacker on contact move", "[Simulation][SimulateTurn][SingleBattle][Item][RockyHelmet]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  auto attacker = test.pokemon(dex::Species::DECIDUEYE, dex::Move::TACKLE);
  types::stat maxHp = computeStatFromBaseStat(
    dex::Stat::HP,
    test.dexValue<dex::Decidueye::hp>(),
    Constants::PokemonLevel::DEFAULT,
    dex::Nature::NO_NATURE);
  attacker.currentHp = maxHp / GENERATE(1U, 2U, 3U);

  test.setupBattle(
    Turn{1U},
    test.side(attacker),
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Item::ROCKY_HELMET, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::TACKLE, dex::Move::SPLASH));

  types::stat hpDivisor = test.dexValue<dex::RockyHelmet::onDamagingHitHpDecreaseDivisor>();

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p1A);
  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);

  stat::CurrentHp currentHp = test.registry().get<stat::CurrentHp>(entities.p1A);
  REQUIRE(currentHp.val == attacker.currentHp.value() - (maxHp / hpDivisor));
}

TEST_CASE(
  "Rocky Helmet: Does not hurt attacker on non-contact move",
  "[Simulation][SimulateTurn][SingleBattle][Item][RockyHelmet]") {
  dex::Move attackerMove = GENERATE(dex::Move::MOONBLAST, dex::Move::LEAFAGE);
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::DECIDUEYE, attackerMove)),
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Item::ROCKY_HELMET, dex::Move::SPLASH)),
    test.turnDecision(attackerMove, dex::Move::SPLASH));

  test.simulateTurnOptions().setRandomChanceLowerLimit(Constants::PercentChance::MAX);
  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(entities.p1A);
  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);
}
}  // namespace pokesim
