#pragma once

#include <entt/entity/fwd.hpp>

namespace pokesim {
struct SideDecision;
struct ActionQueue;
struct Sides;

void resolveDecision(entt::handle sideHandle, const SideDecision& sideDecision, ActionQueue& sideActionQueue);

void moveSideActionsToBattleActions(entt::handle battleHandle, const Sides& sides, ActionQueue& battleActionQueue);
}  // namespace pokesim