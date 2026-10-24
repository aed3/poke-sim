#pragma once

#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;
struct ActionQueue;
struct RecycledAction;
struct SpeedTieIndexes;
struct ActiveWithUpdateEvents;

namespace internal {
struct RandomEventIndex;
}

namespace internal::simulate_turn {
void speedSort(types::handle handle, ActionQueue& actionQueue);
void speedSortActiveWithUpdateEvents(types::handle handle, ActiveWithUpdateEvents& activeWithUpdateEvents);
void speedSortMidTurnSwitches(types::handle handle, ActionQueue& actionQueue);
void resolveActionQueueSpeedTies(Simulation& simulation);
void resolveUpdateEventSpeedTies(Simulation& simulation);

void addBeforeTurnAction(ActionQueue& actionQueue);
void addResidualAction(ActionQueue& actionQueue);
void setCurrentAction(types::handle battleHandle, ActionQueue& actionQueue, RecycledAction action);
}  // namespace internal::simulate_turn
}  // namespace pokesim
