#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("Paralysis: Can cause move failure", "[Simulation][SimulateTurn][SingleBattle][Effect][Status][Paralysis]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Move::SPLASH, dex::Status::PAR)),
    test.side(test.pokemon(dex::Species::RIBOMBEE, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::SPLASH, dex::Move::SPLASH));

  test.simulateTurnOptions().setMakeBranchesOnRandomEvents(true);
  auto battleEntitiesList = test.simulateOneBranchingBattle(Tags<Probability>{});
  REQUIRE(battleEntitiesList.size() == 2U);

  types::probability paralysisChance =
    test.dexValue<dex::Paralysis::onBeforeMoveChance>() * Constants::PercentChanceToProbability;

  const types::registry& registry = test.registry();
  entt::dense_set<types::probability> foundProbabilities;
  for (auto entities : battleEntitiesList) {
    const auto& [turn, probability] = registry.get<Turn, Probability>(entities.battle);

    REQUIRE(turn.val == 2U);
    auto [initialRngSeed, currentRngSeed] = test.checks.getInitialAndCurrent<RngSeed>(entities.battle);
    REQUIRE(currentRngSeed.val == initialRngSeed.val);

    bool paralysisStoppedP1Move = probability.val == Catch::Approx(paralysisChance);
    bool p1Moved = probability.val == Catch::Approx(Constants::Probability::MAX - paralysisChance);

    REQUIRE((paralysisStoppedP1Move || p1Moved));

    if (paralysisStoppedP1Move) {
      test.checks.checkEntityForChanges(entities.p1A);
    }

    if (p1Moved) {
      test.checks.checkUsedMovePokemon(entities.p1A);
    }

    test.checks.checkUsedMovePokemon(entities.p2A);
    foundProbabilities.insert(probability.val);
  }

  REQUIRE(foundProbabilities.size() == 2U);
}
}  // namespace pokesim
