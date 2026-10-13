#include "../Tests.hpp"

namespace pokesim {
TEST_CASE(
  "Levitate: immune to ground-type move", "[Simulation][SimulateTurn][SingleBattle][Ability][Levitate][Immunities]") {
  TestSimulation test{GameMechanics::SCARLET_VIOLET, BattleFormat::SINGLES};
  test.setupBattle(
    Turn{1U},
    test.side(test.pokemon(dex::Species::CLAYDOL, dex::Ability::LEVITATE, dex::Move::SPLASH)),
    test.side(test.pokemon(dex::Species::PANGORO, dex::Move::MUD_SLAP)),
    test.turnDecision(dex::Move::SPLASH, dex::Move::MUD_SLAP));

  auto entities = test.simulateOneNonBranchingBattle();
  test.checks.checkUsedMovePokemon(entities.p1A);
  test.checks.checkUsedMovePokemon(entities.p2A);
}
}  // namespace pokesim
