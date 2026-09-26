#include "../Tests.hpp"

namespace pokesim {
TEST_CASE(
  "Long Reach: Contact moves do not activate contact events",
  "[Simulation][SimulateTurn][SingleBattle][Ability][LongReach]") {
  dex::Move attackerMove = GENERATE(dex::Move::TACKLE, dex::Move::FURY_ATTACK, dex::Move::LEAFAGE);
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::DECIDUEYE, dex::Ability::LONG_REACH, attackerMove)),
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Item::ROCKY_HELMET, dex::Move::SPLASH)),
    test.turnDecision(attackerMove, dex::Move::SPLASH));

  test.simulateTurnOptions().setRandomChanceLowerLimit(Constants::PercentChance::MAX);
  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(entities.p1A);
  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);
}
}  // namespace pokesim
