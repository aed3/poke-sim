#include "Tests.hpp"

namespace pokesim {
TEST_CASE("Analyze Effect: Vertical Slice 1", "[Simulation][AnalyzeEffect]") {
  Simulation::BattleCreationInfo battleCreationInfo{};
  Simulation simulation = createSingleBattleSimulation(battleCreationInfo, false);

  battleCreationInfo.runWithAnalyzeEffect = true;
  battleCreationInfo.effectsToAnalyze = {{
    Slot::P1A,
    Slot::P2A,
    Slot::P1A,
    {dex::Move::FURY_ATTACK},
    dex::Status::BRN,
  }};
  simulation.createInitialStates({battleCreationInfo});
  auto result = simulation.analyzeEffect();
}
}  // namespace pokesim