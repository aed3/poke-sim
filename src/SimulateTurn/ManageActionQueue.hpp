#pragma once

#include <Types/Registry.hpp>

// Systems
namespace pokesim {
class Simulation;
struct SideDecision;
struct ActionQueue;
struct RecycledAction;
struct SpeedTieIndexes;

namespace internal {
struct RandomEventIndex;
}

namespace internal::simulate_turn {
void resolveDecision(types::handle sideHandle, const SideDecision& sideDecision);
void speedSort(types::handle handle, ActionQueue& actionQueue);
void resolveSpeedTies(Simulation& simulation);
void setSpeedTieOrder(ActionQueue& actionQueue, const SpeedTieIndexes& speedTies, RandomEventIndex randomEventIndex);

void addBeforeTurnAction(ActionQueue& actionQueue);
void addResidualAction(ActionQueue& actionQueue);
void setCurrentAction(types::handle battleHandle, ActionQueue& actionQueue, RecycledAction action);
void clearActionQueue(ActionQueue& actionQueue);
}  // namespace internal::simulate_turn
}  // namespace pokesim
