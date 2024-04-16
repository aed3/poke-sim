#pragma once

#include <Types/Entity.hpp>

// Systems
namespace pokesim {
struct SideDecision;
struct ActionQueue;

namespace simulate_turn {
void resolveDecision(types::handle sideHandle, const SideDecision& sideDecision);
void speedSort(types::handle handle, ActionQueue& actionQueue);

void addBeforeTurnAction(types::handle handle, ActionQueue& actionQueue);
void addResidualAction(types::handle handle, ActionQueue& actionQueue);
void setActiveAction(types::handle handle, ActionQueue& actionQueue);
}  // namespace simulate_turn
}  // namespace pokesim