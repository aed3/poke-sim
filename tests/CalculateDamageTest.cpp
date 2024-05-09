#include "Tests.hpp"

namespace pokesim {
TEST_CASE("Calculate Damage: Vertical Slice 1", "[Simulation][CalculateDamage]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  Simulation simulation = createSingleBattleSimulation(battleCreationInfo, false);

  battleCreationInfo.runWithCalculateDamage = true;
  simulation.createInitialStates({battleCreationInfo});
  simulation.calculateDamage();
}
}  // namespace pokesim