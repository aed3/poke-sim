#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("Ally Switch: Does nothing in singles", "[Simulation][SimulateTurn][SingleBattle][Move][AllySwitch]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::CLAYDOL, dex::Move::ALLY_SWITCH)),
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::ALLY_SWITCH, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);
}

TEST_CASE("Ally Switch: Switches slots", "[Simulation][SimulateTurn][DoubleBattle][Move][AllySwitch]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::DOUBLES};
  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::CLAYDOL, dex::Move::ALLY_SWITCH, dex::Move::SPLASH),
      test.pokemon(dex::Species::GRIMMSNARL, dex::Move::SPLASH)),
    test.side(
      test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH),
      test.pokemon(dex::Species::AMPHAROS, dex::Move::THUNDERBOLT, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::SPLASH, dex::Move::SPLASH, dex::Move::SPLASH, dex::Move::SPLASH));

  auto beforeAllySwitch = test.simulateOneNonBranchingBattle();

  test.applyDecision(
    beforeAllySwitch.battle,
    test.turnDecision(dex::Move::ALLY_SWITCH, dex::Move::SPLASH, dex::Move::SPLASH, dex::Move::THUNDERBOLT));

  auto afterAllySwitch = test.simulateOneNonBranchingBattle({}, Tags<Team>{});
  test.checks.checkUsedMovePokemon(afterAllySwitch.p1A);
  test.checks.checkUsedMovePokemon<dex::AllySwitch::Effect>(afterAllySwitch.p1B);
  test.checks.checkUsedMovePokemon(afterAllySwitch.p2A);
  test.checks.checkUsedMovePokemon(afterAllySwitch.p2B);

  REQUIRE(beforeAllySwitch.p1A == afterAllySwitch.p1B);
  REQUIRE(beforeAllySwitch.p1B == afterAllySwitch.p1A);

  const dex::AllySwitch::Effect& allySwitch = test.registry().get<dex::AllySwitch::Effect>(afterAllySwitch.p1B);
  REQUIRE(allySwitch.turnsRemaining == 1U);
  REQUIRE(allySwitch.uses == 1U);
}

TEST_CASE(
  "Ally Switch: Has progressively less chance to work when used successfully",
  "[Simulation][SimulateTurn][DoubleBattle][Move][AllySwitch]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::DOUBLES};
  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::CLAYDOL, dex::Move::ALLY_SWITCH, dex::Move::SPLASH),
      test.pokemon(dex::Species::GRIMMSNARL, dex::Move::SPLASH)),
    test.side(
      test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH),
      test.pokemon(dex::Species::AMPHAROS, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::ALLY_SWITCH, dex::Move::SPLASH, dex::Move::SPLASH, dex::Move::SPLASH));
  types::registry& registry = test.registry();

  Slot allySwitchUser = Slot::P1B;
  types::probability idealProbability = Constants::Probability::MAX;
  types::entity keptBattle = test.simulateOneNonBranchingBattle({}, Tags<Team>{}).battle;

  for (types::stallUses uses = 1U; uses <= Constants::StallMoveUses::MAX; uses++) {
    CAPTURE(uses);

    auto turnDecision = test.turnDecision(
      allySwitchUser == Slot::P1A ? dex::Move::ALLY_SWITCH : dex::Move::SPLASH,
      allySwitchUser == Slot::P1B ? dex::Move::ALLY_SWITCH : dex::Move::SPLASH,
      dex::Move::SPLASH,
      dex::Move::SPLASH);
    test.applyDecision(keptBattle, turnDecision);
    auto battleEntitiesList = test.simulateOneBranchingBattle({}, Tags<Team>{});
    REQUIRE(battleEntitiesList.size() == 2U);

    TestSimulation::BattleEntities allySwitchPassed{entt::null}, allySwitchFailed{entt::null};
    for (auto entities : battleEntitiesList) {
      test.checks.checkUsedMovePokemon<dex::AllySwitch::Effect>(entities.p1A);
      test.checks.checkUsedMovePokemon<dex::AllySwitch::Effect>(entities.p1B);
      test.checks.checkUsedMovePokemon(entities.p2A);
      test.checks.checkUsedMovePokemon(entities.p2B);

      dex::AllySwitch::Effect* allySwitch =
        registry.try_get<dex::AllySwitch::Effect>(allySwitchUser == Slot::P1A ? entities.p1B : entities.p1A);
      if (allySwitch) {
        allySwitchPassed = entities;
        REQUIRE(allySwitch->uses == std::min((types::stallUses)(uses + 1U), Constants::StallMoveUses::MAX));
        REQUIRE(allySwitch->turnsRemaining == 1U);
      }
      else {
        allySwitchFailed = entities;
      }
    }

    REQUIRE_FALSE((allySwitchPassed.battle == entt::null));
    REQUIRE_FALSE((allySwitchFailed.battle == entt::null));
    types::probability succeedProbability = 1.0F;
    switch (uses) {
      case 1U: succeedProbability /= 3U; break;
      case 2U: succeedProbability /= 9U; break;
      case 3U: succeedProbability /= 27U; break;
      case 4U: succeedProbability /= 81U; break;
      case 5U: succeedProbability /= 243U; break;
      case 6U:
      case 7U: succeedProbability /= 729U; break;
      default: FAIL();
    }

    REQUIRE_THAT(
      registry.get<Probability>(allySwitchPassed.battle).val,
      Catch::Matchers::WithinRel(idealProbability * succeedProbability));
    REQUIRE_THAT(
      registry.get<Probability>(allySwitchFailed.battle).val,
      Catch::Matchers::WithinRel(idealProbability * (Constants::Probability::MAX - succeedProbability)));

    allySwitchUser = allySwitchUser == Slot::P1A ? Slot::P1B : Slot::P1A;
    idealProbability *= succeedProbability;
    keptBattle = allySwitchPassed.battle;
    registry.remove<tags::SimulateTurn>(allySwitchFailed.battle);
  }

  test.applyDecision(
    keptBattle,
    test.turnDecision(dex::Move::SPLASH, dex::Move::SPLASH, dex::Move::SPLASH, dex::Move::SPLASH));
  auto entities = test.simulateOneNonBranchingBattle();
  REQUIRE_FALSE(registry.all_of<dex::AllySwitch::Effect>(entities.p1A));
  REQUIRE_FALSE(registry.all_of<dex::AllySwitch::Effect>(entities.p1B));
}
}  // namespace pokesim
