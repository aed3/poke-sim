#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("Static", "[Simulation][SimulateTurn][SingleBattle][Ability][Static][Paralysis]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::TACKLE)),
    test.side(test.pokemon(dex::Species::AMPHAROS, dex::Ability::STATIC, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::TACKLE, dex::Move::SPLASH));

  test.simulateTurnOptions().setMakeBranchesOnRandomEvents(true);
  auto battleEntitiesList = test.simulateOneBranchingBattle();
  REQUIRE(battleEntitiesList.size() == 2U);

  types::probability staticChance =
    test.dexValue<dex::Static::onDamagingHitChance>() * Constants::PercentChanceToProbability;

  const types::registry& registry = test.registry();
  entt::dense_set<types::probability> foundProbabilities;
  for (auto entities : battleEntitiesList) {
    Probability probability = registry.get<Probability>(entities.battle);

    bool p1Paralyzed = registry.all_of<dex::Paralysis>(entities.p1A);

    if (p1Paralyzed) {
      REQUIRE_THAT(probability.val, Catch::Matchers::WithinRel(staticChance));
      test.checks.checkUsedMovePokemon<dex::Paralysis, tags::HasStatus, stat::EffectiveSpe>(entities.p1A);
      auto [initialSpe, currentSpe] = test.checks.getInitialAndCurrent<stat::EffectiveSpe>(entities.p1A);
      REQUIRE(registry.all_of<tags::HasStatus>(entities.p1A));
      REQUIRE(
        currentSpe.val == initialSpe.val / (test.dexValue<dex::Paralysis::speedDivisor>() /
                                            test.dexValue<dex::Paralysis::speedDividend>()));
    }
    else {
      REQUIRE_THAT(probability.val, Catch::Matchers::WithinRel(Constants::Probability::MAX - staticChance));
      test.checks.checkUsedMovePokemon(entities.p1A);
    }

    test.checks.checkUsedMovePokemon<stat::CurrentHp>(entities.p2A);
    foundProbabilities.insert(probability.val);
  }

  REQUIRE(foundProbabilities.size() == 2U);
}
}  // namespace pokesim
