#pragma once

#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;
struct ActionQueue;
struct ActiveWithUpdateEvents;

namespace internal::simulate_turn {
void speedSort(types::handle handle, ActionQueue& actionQueue);
void speedSortActiveWithUpdateEvents(types::handle handle, ActiveWithUpdateEvents& activeWithUpdateEvents);
void speedSortMidTurnSwitches(types::handle handle, ActionQueue& actionQueue);
void resolveActionQueueSpeedTies(Simulation& simulation);
void resolveUpdateEventSpeedTies(Simulation& simulation);
}  // namespace internal::simulate_turn
}  // namespace pokesim
