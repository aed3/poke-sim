#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
struct SideDecision;
struct ActionQueue;

void resolveDecision(types::handle sideHandle, const SideDecision& sideDecision);
void speedSort(types::handle handle, ActionQueue& actionQueue);
}  // namespace pokesim