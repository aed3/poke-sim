#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("Knock Off: Remove Most Items", "[Simulation][SimulateTurn][SingleBattle][Move][KnockOff]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::KNOCK_OFF)),
    test.side(test.pokemon(dex::Species::RIBOMBEE, dex::Item::LIFE_ORB, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::KNOCK_OFF, dex::Move::SPLASH));

  test.simulateTurnOptions().setDamageRollsConsidered(
    {DamageRollKind::AVERAGE_DAMAGE | DamageRollKind::GUARANTEED_CRIT_CHANCE});
  auto entities = test.simulateOneNonBranchingBattle();

  test.checks.checkUsedMovePokemon(entities.p1A);
  test.checks.checkUsedMovePokemon<stat::CurrentHp, tags::HasItem, dex::LifeOrb>(entities.p2A);

  auto [initialP2Hp, currentP2Hp] = test.checks.getInitialAndCurrent<stat::CurrentHp>(entities.p2A);
  REQUIRE(currentP2Hp.val < initialP2Hp.val);

  REQUIRE_FALSE(test.registry().all_of<tags::HasItem>(entities.p2A));
  REQUIRE_FALSE(test.registry().all_of<dex::LifeOrb>(entities.p2A));
};
}  // namespace pokesim
