#pragma once

#include <Types/Registry.hpp>

// Systems
namespace pokesim {
struct SideDecision;
struct ActionQueue;
struct RecycledAction;

namespace simulate_turn {
void resolveDecision(types::handle sideHandle, const SideDecision& sideDecision);
void speedSort(types::handle handle, ActionQueue& actionQueue);

void addBeforeTurnAction(ActionQueue& actionQueue);
void addResidualAction(ActionQueue& actionQueue);
void setCurrentAction(types::handle battleHandle, ActionQueue& actionQueue, RecycledAction& action);
void clearActionQueue(ActionQueue& actionQueue);
}  // namespace simulate_turn
}  // namespace pokesim
