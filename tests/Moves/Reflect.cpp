#include "../Tests.hpp"

namespace pokesim {
TEST_CASE(
  "Reflect: Decrease damage of physical moves",
  "[Simulation][SimulateTurn][SingleBattle][Move][Reflect][Effect][SideCondition]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::GRIMMSNARL, dex::Move::REFLECT)),
    test.side(test.pokemon(dex::Species::AMPHAROS, dex::Move::TACKLE)),
    test.turnDecision(dex::Move::REFLECT, dex::Move::TACKLE));

  auto entities = test.simulateOneNonBranchingBattle({}, Tags<dex::Reflect::Effect>{});

  REQUIRE(test.registry().all_of<dex::Reflect::Effect>(entities.p1Side));
  REQUIRE_FALSE(test.registry().all_of<dex::Reflect::Effect>(entities.p2Side));
  REQUIRE(
    test.registry().get<dex::Reflect::Effect>(entities.p1Side).val ==
    test.dexValue<dex::Reflect::Effect::duration>() - 1U);

  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);

  REQUIRE(test.registry().get<stat::CurrentHp>(entities.p1A).val == 280U);
}

TEST_CASE(
  "Reflect: Do not decrease damage of special moves",
  "[Simulation][SimulateTurn][SingleBattle][Move][Reflect][Effect][SideCondition]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::GRIMMSNARL, dex::Move::REFLECT)),
    test.side(test.pokemon(dex::Species::AMPHAROS, dex::Move::THUNDERBOLT)),
    test.turnDecision(dex::Move::REFLECT, dex::Move::THUNDERBOLT));

  test.simulateTurnOptions().setRandomChanceLowerLimit(Constants::PercentChance::MAX);
  auto entities = test.simulateOneNonBranchingBattle({}, Tags<dex::Reflect::Effect>{});

  REQUIRE(test.registry().all_of<dex::Reflect::Effect>(entities.p1Side));
  REQUIRE_FALSE(test.registry().all_of<dex::Reflect::Effect>(entities.p2Side));
  REQUIRE(
    test.registry().get<dex::Reflect::Effect>(entities.p1Side).val ==
    test.dexValue<dex::Reflect::Effect::duration>() - 1U);

  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);

  REQUIRE(test.registry().get<stat::CurrentHp>(entities.p1A).val == 126U);
}

TEST_CASE(
  "Reflect: Remove after 5 turns", "[Simulation][SimulateTurn][SingleBattle][Move][Reflect][Effect][SideCondition]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::GRIMMSNARL, dex::Move::REFLECT, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::AMPHAROS, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::REFLECT, dex::Move::SPLASH));

  types::effectDuration start = 1U + Constants::EffectDuration::MIN;
  types::effectDuration duration = test.dexValue<dex::Reflect::Effect::duration>();
  for (types::effectDuration i = start; i < duration; i++) {
    auto entities = test.simulateOneNonBranchingBattle({}, Tags<dex::Reflect::Effect>{});

    test.checks.checkUsedMovePokemon(entities.p1A, i == start ? 0U : 1U);
    test.checks.checkUsedMovePokemon(entities.p2A);

    REQUIRE(test.registry().get<Turn>(entities.battle).val == i + 1U);

    REQUIRE(test.registry().all_of<dex::Reflect::Effect>(entities.p1Side));
    REQUIRE_FALSE(test.registry().all_of<dex::Reflect::Effect>(entities.p2Side));
    REQUIRE(test.registry().get<dex::Reflect::Effect>(entities.p1Side).val == duration - i);

    test.applyDecision(entities.battle, test.turnDecision(dex::Move::SPLASH, dex::Move::SPLASH));
  }

  auto entities = test.simulateOneNonBranchingBattle({}, Tags<dex::Reflect::Effect>{});
  REQUIRE_FALSE(test.registry().all_of<dex::Reflect::Effect>(entities.p1Side));
  REQUIRE_FALSE(test.registry().all_of<dex::Reflect::Effect>(entities.p2Side));
}

TEST_CASE(
  "Reflect: Decrease damage of physical moves less in doubles",
  "[Simulation][SimulateTurn][DoubleBattle][Move][Reflect][Effect][SideCondition]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::DOUBLES};
  test.setupBattle(
    Turn{1U},
    test.side(
      test.pokemon(dex::Species::EMPOLEON, dex::Move::TACKLE),
      test.pokemon(dex::Species::GARDEVOIR, dex::Move::TACKLE),
      dex::SideCondition::REFLECT),
    test.side(
      test.pokemon(dex::Species::AMPHAROS, dex::Move::TACKLE),
      test.pokemon(dex::Species::PANGORO, dex::Move::SPLASH)),
    test.turnDecision(
      MoveDecision{Slot::P1A, Slot::P1B, dex::Move::TACKLE},
      dex::Move::TACKLE,
      dex::Move::TACKLE,
      dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle({}, Tags<dex::Reflect::Effect>{});

  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p1A);
  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p1B);
  test.checks.checkUsedMovePokemon(entities.p2A);
  test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2B);
  test.checks.checkUsedMovePokemon(entities.p2A);

  REQUIRE(test.registry().get<stat::CurrentHp>(entities.p1A).val == 268U);
  REQUIRE(test.registry().get<stat::CurrentHp>(entities.p1B).val == 215U);
  REQUIRE(test.registry().get<stat::CurrentHp>(entities.p2B).val == 270U);
}

TEST_CASE(
  "Reflect: Works with AnalyzeEffect",
  "[Simulation][AnalyzeEffect][SingleBattle][Move][Reflect][Effect][SideCondition]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::AMPHAROS, dex::Move::SPLASH)),
    AnalyzeEffectInputInfo{
      Slot::P1A,
      Slot::P2A,
      Slot::P2A,
      {
        dex::Move::TACKLE,
      },
      dex::SideCondition::REFLECT,
    });

  auto result = test.analyzeEffect();
  result.effectMultiplierResults().each([](analyze_effect::EffectMultiplier multiplier) {
    REQUIRE_THAT(multiplier.val, Catch::Matchers::WithinAbs(0.5F, 0.05F));
  });
}
}  // namespace pokesim
