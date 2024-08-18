#include "Tests.hpp"

namespace pokesim {
TEST_CASE("Calculate Damage: Vertical Slice 1", "[Simulation][CalculateDamage]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  Simulation simulation = createSingleBattleSimulation(battleCreationInfo, false);

  battleCreationInfo.runWithCalculateDamage = true;
  battleCreationInfo.damageCalculations = {
    {Slot::P1A, Slot::P2A, dex::Move::FURY_ATTACK},
    {Slot::P2A, Slot::P1A, dex::Move::THUNDERBOLT},
  };
  simulation.createInitialStates({battleCreationInfo});
  simulation.calculateDamage();
}
}  // namespace pokesim