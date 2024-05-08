#include "Tests.hpp"

namespace pokesim {
TEST_CASE("Analyze Effect: Vertical Slice 1", "[Simulation][AnalyzeEffect]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  Simulation simulation = createSingleBattleSimulation(battleCreationInfo);

  battleCreationInfo.runWithAnalyzeEffect = true;
  simulation.analyzeEffect();
}
}  // namespace pokesim