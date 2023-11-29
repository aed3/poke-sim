#pragma once

#include <entt/entity/fwd.hpp>

namespace pokesim {
struct SideDecision;
struct ActionQueue;
struct Sides;

/*_inline_*/ void resolveDecision(entt::handle sideHandle, const SideDecision& sideDecision, ActionQueue& sideActionQueue);

/*_inline_*/ void moveSideActionsToBattleActions(entt::handle battleHandle, const Sides& sides, ActionQueue& battleActionQueue);
}  // namespace pokesim