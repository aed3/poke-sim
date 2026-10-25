#pragma once

#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;
struct ActionQueue;
struct RecycledAction;

namespace internal::simulate_turn {
void addBeforeTurnAction(ActionQueue& actionQueue);
void addResidualAction(ActionQueue& actionQueue);
void setCurrentAction(types::handle battleHandle, ActionQueue& actionQueue, RecycledAction action);
}  // namespace internal::simulate_turn
}  // namespace pokesim
