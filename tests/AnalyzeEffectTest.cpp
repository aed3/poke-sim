#include "Tests.hpp"

namespace pokesim {
TEST_CASE("Analyze Effect: Vertical Slice 1", "[Simulation][AnalyzeEffect]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  Simulation simulation = createSingleBattleSimulation(battleCreationInfo, false);

  battleCreationInfo.runWithAnalyzeEffect = true;
  simulation.createInitialStates({battleCreationInfo});
  simulation.analyzeEffect();
}
}  // namespace pokesim