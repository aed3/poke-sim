#pragma once

#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;
struct SideDecision;
struct MidTurnSideDecision;

namespace simulate_turn {
void setSideOptions(Simulation& simulation);
void setTeamPreviewOptions(Simulation& simulation);
}  // namespace simulate_turn

namespace internal::simulate_turn {
void resolveDecision(types::handle sideHandle, const SideDecision& sideDecision);
void resolveMidTurnDecisions(types::handle sideHandle, const MidTurnSideDecision& switchDecisions);
}  // namespace internal::simulate_turn
}  // namespace pokesim
