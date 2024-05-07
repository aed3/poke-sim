#include "Tests.hpp"

namespace pokesim {
TEST_CASE("Calculate Damage: Vertical Slice 1", "[Simulation][CalculateDamage]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  Simulation simulation = createSingleBattleSimulation(battleCreationInfo);

  battleCreationInfo.runWithCalculateDamage = true;
  simulation.calculateDamage();
}
}  // namespace pokesim