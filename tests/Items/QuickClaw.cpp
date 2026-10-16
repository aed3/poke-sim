#include "../Tests.hpp"

namespace pokesim {
constexpr types::probability QUICK_CLAW_PROBABILITY =
  dex::QuickClaw::onFractionalPriorityChance(GameMechanics::SCARLET_VIOLET) * Constants::PercentChanceToProbability;
constexpr types::probability NO_QUICK_CLAW_PROBABILITY = Constants::Probability::MAX - QUICK_CLAW_PROBABILITY;

TEST_CASE(
  "Quick Claw: Randomly increase priority of moves",
  "[Simulation][SimulateTurn][SingleBattle][Items][QuickClaw][Priority]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Item::QUICK_CLAW, dex::Move::GROWL)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Move::TACKLE)),
    test.turnDecision(dex::Move::GROWL, dex::Move::TACKLE));

  auto battleEntitiesList = test.simulateOneBranchingBattle();
  REQUIRE(battleEntitiesList.size() == 2U);

  types::stat hpAfterBaseAttack = 255U;
  types::stat hpAfterMinusOneAttack = 262U;

  entt::dense_set<types::probability> foundProbabilities;
  for (auto entities : battleEntitiesList) {
    test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p1A);
    test.checks.checkUsedMovePokemon<AtkBoost, stat::EffectiveAtk>(entities.p2A);
    types::probability probability = test.registry().get<Probability>(entities.battle).val;
    CAPTURE(probability);

    if (Catch::Matchers::WithinRel(probability).match(QUICK_CLAW_PROBABILITY)) {
      REQUIRE(test.registry().get<stat::CurrentHp>(entities.p1A).val == hpAfterMinusOneAttack);
    }
    else if (Catch::Matchers::WithinRel(probability).match(NO_QUICK_CLAW_PROBABILITY)) {
      REQUIRE(test.registry().get<stat::CurrentHp>(entities.p1A).val == hpAfterBaseAttack);
    }
    else {
      FAIL();
    }
    foundProbabilities.insert(probability);
  }

  REQUIRE(foundProbabilities.size() == 2U);
}

TEST_CASE(
  "Quick Claw: Stay within priority bracket", "[Simulation][SimulateTurn][SingleBattle][Item][QuickClaw][Priority]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::AMPHAROS, dex::Item::QUICK_CLAW, dex::Move::GROWL)),
    test.side(test.pokemon(dex::Species::AMPHAROS, dex::Move::QUICK_ATTACK)),
    test.turnDecision(dex::Move::GROWL, dex::Move::QUICK_ATTACK));

  auto battleEntitiesList = test.simulateOneBranchingBattle();
  REQUIRE(battleEntitiesList.size() == 2U);

  types::stat hpAfterBaseAttack = 259U;
  entt::dense_set<types::probability> foundProbabilities;
  for (auto entities : battleEntitiesList) {
    test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p1A);
    test.checks.checkUsedMovePokemon<AtkBoost, stat::EffectiveAtk>(entities.p2A);
    types::probability probability = test.registry().get<Probability>(entities.battle).val;
    CAPTURE(probability);

    REQUIRE_THAT(
      probability,
      Catch::Matchers::WithinRel(QUICK_CLAW_PROBABILITY) || Catch::Matchers::WithinRel(NO_QUICK_CLAW_PROBABILITY));
    REQUIRE(test.registry().get<stat::CurrentHp>(entities.p1A).val == hpAfterBaseAttack);
    foundProbabilities.insert(probability);
  }

  REQUIRE(foundProbabilities.size() == 2U);
}

TEST_CASE(
  "Quick Claw: Only activate if moving this turn", "[Simulation][SimulateTurn][SingleBattle][Item][QuickClaw]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::EMPOLEON, dex::Item::QUICK_CLAW, dex::Move::SPLASH),
      test.pokemon(dex::Species::GARDEVOIR, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::DRAGAPULT, dex::Move::SPLASH)),
    test.turnDecision(Slot::P1B, dex::Move::SPLASH));

  test.simulateOneNonBranchingBattle({}, Tags<Team>{});
}
}  // namespace pokesim
