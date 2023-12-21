#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
struct SideDecision;
struct ActionQueue;
struct Sides;

/*_inline_*/ void resolveDecision(
  types::handle sideHandle, const SideDecision& sideDecision, ActionQueue& sideActionQueue);

/*_inline_*/ void moveSideActionsToBattleActions(
  types::handle battleHandle, const Sides& sides, ActionQueue& battleActionQueue);
}  // namespace pokesim