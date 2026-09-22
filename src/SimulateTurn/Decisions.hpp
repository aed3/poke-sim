#pragma once

#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;
struct Battle;
struct SideDecision;
struct MidTurnSideDecision;
struct MidTurnDecisionsRequested;

namespace simulate_turn {
void setSideOptions(Simulation& simulation);
void setTeamPreviewOptions(Simulation& simulation);
}  // namespace simulate_turn

namespace internal::simulate_turn {
void resolveDecision(types::handle sideHandle, Battle battle, const SideDecision& sideDecision);
void resolveMidTurnDecisions(
  types::handle sideHandle, Battle battle, const MidTurnSideDecision& switchDecisions,
  MidTurnDecisionsRequested& decisionsRequested);
}  // namespace internal::simulate_turn
}  // namespace pokesim
