#include "../Tests.hpp"

namespace pokesim {
TEST_CASE(
  "Prankster: Increase priority of status moves",
  "[Simulation][SimulateTurn][SingleBattle][Ability][Prankster][Priority]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::GRIMMSNARL, dex::Ability::PRANKSTER, dex::Move::GROWL)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Move::TACKLE)),
    test.turnDecision(dex::Move::GROWL, dex::Move::TACKLE));

  auto entities = test.simulateOneNonBranchingBattle();

  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p1A);
  test.checks.checkUsedMovePokemon<AtkBoost, stat::EffectiveAtk>(entities.p2A);
  types::stat hpAfterMinusOneAttack = 258U;

  REQUIRE(test.registry().get<stat::CurrentHp>(entities.p1A).val == hpAfterMinusOneAttack);
}

TEST_CASE(
  "Prankster: Maintain speed order for moves of the same priority",
  "[Simulation][SimulateTurn][SingleBattle][Ability][Prankster][Priority]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::GRIMMSNARL, dex::Ability::PRANKSTER, dex::Move::GROWL)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Move::QUICK_ATTACK)),
    test.turnDecision(dex::Move::GROWL, dex::Move::QUICK_ATTACK));

  auto entities = test.simulateOneNonBranchingBattle();

  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p1A);
  test.checks.checkUsedMovePokemon<AtkBoost, stat::EffectiveAtk>(entities.p2A);
  types::stat hpAfterBaseAttack = 238U;

  REQUIRE(test.registry().get<stat::CurrentHp>(entities.p1A).val == hpAfterBaseAttack);
}

TEST_CASE(
  "Prankster: Only boosts status moves", "[Simulation][SimulateTurn][SingleBattle][Ability][Prankster][Priority]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::GRIMMSNARL, dex::Ability::PRANKSTER, dex::Move::CHILLING_WATER)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Move::TACKLE)),
    test.turnDecision(dex::Move::CHILLING_WATER, dex::Move::TACKLE));

  auto entities = test.simulateOneNonBranchingBattle();

  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p1A);
  test.checks.checkUsedMovePokemon<stat::CurrentHp, AtkBoost, stat::EffectiveAtk>(entities.p2A);
  types::stat hpAfterBaseAttack = 238U;

  REQUIRE(test.registry().get<stat::CurrentHp>(entities.p1A).val == hpAfterBaseAttack);
}

TEST_CASE(
  "Prankster: Dark types are immune", "[Simulation][SimulateTurn][SingleBattle][Ability][Prankster][Priority]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  dex::Move p1Move = GENERATE(dex::Move::GROWL, dex::Move::WILL_O_WISP, dex::Move::NOBLE_ROAR);
  CAPTURE(p1Move);
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::GRIMMSNARL, dex::Ability::PRANKSTER, p1Move)),
    test.side(test.pokemon(dex::Species::GRIMMSNARL, dex::Ability::PRANKSTER, dex::Move::GROWL)),
    test.turnDecision(p1Move, dex::Move::GROWL));

  auto battleEntitiesList = test.simulateOneBranchingBattle();

  for (auto entities : battleEntitiesList) {
    test.checks.checkUsedMovePokemon(entities.p1A);
    test.checks.checkUsedMovePokemon(entities.p2A);
  }
}
}  // namespace pokesim
