#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
struct SideDecision;
struct ActionQueue;

void resolveDecision(types::handle sideHandle, const SideDecision& sideDecision, ActionQueue& sideActionQueue);
}  // namespace pokesim