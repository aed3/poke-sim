#include "../Tests.hpp"

namespace pokesim {
TEST_CASE(
  "Scrappy: Ignores normal and fighting type immunities",
  "[Simulation][SimulateTurn][SingleBattle][Ability][Scrappy][Immunities]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::HISUIAN_DECIDUEYE, dex::Ability::SCRAPPY, dex::Move::TACKLE)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Item::ROCKY_HELMET, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::TACKLE, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p1A);
  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);

  auto [p1InitialHp, p1CurrentHp] = test.checks.getInitialAndCurrent<stat::CurrentHp>(entities.p1A);
  REQUIRE(p1CurrentHp.val < p1InitialHp.val);
  auto [p2InitialHp, p2CurrentHp] = test.checks.getInitialAndCurrent<stat::CurrentHp>(entities.p2A);
  REQUIRE(p2CurrentHp.val < p2InitialHp.val);
}

TEST_CASE(
  "Scrappy: Does not ignores other immunities",
  "[Simulation][SimulateTurn][SingleBattle][Ability][Scrappy][Immunities]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::HISUIAN_DECIDUEYE, dex::Ability::SCRAPPY, dex::Move::THUNDERBOLT)),
    test.side(test.pokemon(dex::Species::CLAYDOL, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::THUNDERBOLT, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);
}
}  // namespace pokesim
