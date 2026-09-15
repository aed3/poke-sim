#include "../Tests.hpp"

namespace pokesim {
TEST_CASE("Quick Powder: Only increases Ditto's speed", "[Simulation][SimulateTurn][SingleBattle][Item][QuickPowder]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::DITTO, dex::Ability::CLEAR_BODY, dex::Item::QUICK_POWDER, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::EMPOLEON, dex::Item::QUICK_POWDER, dex::Move::SPLASH)),
    test.turnDecision(dex::Move::SPLASH, dex::Move::SPLASH));

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);

  auto speedMultiplier = test.dexValue<dex::QuickPowder::onModifySpeModifier>();
  auto [p1EffectiveSpe, p1Spe] = test.registry().get<stat::EffectiveSpe, stat::Spe>(entities.p1A);
  auto [p2EffectiveSpe, p2Spe] = test.registry().get<stat::EffectiveSpe, stat::Spe>(entities.p2A);

  REQUIRE(p1EffectiveSpe.val == p1Spe.val * speedMultiplier);
  REQUIRE(p2EffectiveSpe.val == p2Spe.val);
}
}  // namespace pokesim
