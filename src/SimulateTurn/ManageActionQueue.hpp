#pragma once

#include <Types/Registry.hpp>

// Systems
namespace pokesim {
struct SideDecision;
struct ActionQueue;

namespace simulate_turn {
void resolveDecision(types::handle sideHandle, const SideDecision& sideDecision);
void speedSort(types::handle handle, ActionQueue& actionQueue);

void addBeforeTurnAction(types::registry& registry, ActionQueue& actionQueue);
void addResidualAction(types::registry& registry, ActionQueue& actionQueue);
void setCurrentAction(types::handle battleHandle, ActionQueue& actionQueue);
}  // namespace simulate_turn
}  // namespace pokesim