#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("Choice Lock: Choice lock starts", "[Simulation][SimulateTurn][SingleBattle][Effect][Volatile][ChoiceLock]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH)),
    test.side(test.pokemon(
      dex::Species::RIBOMBEE,
      dex::Item::CHOICE_SPECS,
      dex::Move::MOONBLAST,
      dex::Move::SPLASH,
      dex::Move::QUIVER_DANCE)),
    test.turnDecision(dex::Move::SPLASH, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle();
  types::moveSlotIndex p2MoveIndex = 1U;

  test.checks.checkUsedMovePokemon(entities.p1A);
  test.checks.checkUsedMovePokemon<DisabledMoveSlots, ChoiceLock>(entities.p2A, p2MoveIndex);

  auto choiceLock = test.registry().get<ChoiceLock>(entities.p2A);
  REQUIRE(choiceLock.val == p2MoveIndex);

  auto disabledMoveSlots = test.registry().get<DisabledMoveSlots>(entities.p2A);
  for (types::moveSlotIndex i = 0U; i < disabledMoveSlots.val.size(); i++) {
    if (i == p2MoveIndex) {
      REQUIRE(disabledMoveSlots.val[i]);
    }
    else {
      REQUIRE_FALSE(disabledMoveSlots.val[i]);
    }
  }
}

TEST_CASE(
  "Choice Lock: Choice lock ends when item is removed",
  "[Simulation][SimulateTurn][SingleBattle][Effect][Volatile][ChoiceLock]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::KNOCK_OFF)),
    test.side(test.pokemon(dex::Species::RIBOMBEE, dex::Item::CHOICE_SCARF, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::KNOCK_OFF, dex::Move::SPLASH));

  test.simulateTurnOptions().setDamageRollsConsidered(
    {DamageRollKind::AVERAGE_DAMAGE | DamageRollKind::GUARANTEED_CRIT_CHANCE});
  auto entities = test.simulateOneNonBranchingBattle();

  test.checks.checkUsedMovePokemon(entities.p1A);
  test.checks
    .checkUsedMovePokemon<tags::HasItem, dex::ChoiceScarf, item::tags::Choice, stat::EffectiveSpe, stat::CurrentHp>(
      entities.p2A);

  const types::registry& registry = test.registry();
  REQUIRE_FALSE(registry.all_of<tags::HasItem>(entities.p2A));
  REQUIRE_FALSE(registry.all_of<dex::ChoiceScarf>(entities.p2A));
  REQUIRE_FALSE(registry.all_of<item::tags::Choice>(entities.p2A));

  auto [initialSpe, currentSpe] = test.checks.getInitialAndCurrent<stat::EffectiveSpe>(entities.p2A);
  REQUIRE(initialSpe.val > currentSpe.val);
}
}  // namespace pokesim
