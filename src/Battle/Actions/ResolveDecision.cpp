#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Types/Entity.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

#include "Action.hpp"
#include "Decisions.hpp"

namespace pokesim {
void resolveDecision(
  types::handle /*sideHandle*/, const SideDecision& /*sideDecision*/, ActionQueue& /*sideActionQueue*/) {
  // TODO(aed3): Turn the sideDecision into a set of components that will be assigned to a new entity that's added to
  // the sideActionQueue
}

void moveSideActionsToBattleActions(types::handle battleHandle, const Sides& sides, ActionQueue& battleActionQueue) {
  types::registry* registry = battleHandle.registry();
  ActionQueue& p1Actions = registry->get<ActionQueue>(sides.p1);
  ActionQueue& p2Actions = registry->get<ActionQueue>(sides.p2);

  battleActionQueue.actionQueue.insert(
    battleActionQueue.actionQueue.end(),
    std::make_move_iterator(p1Actions.actionQueue.begin()),
    std::make_move_iterator(p1Actions.actionQueue.end()));

  p1Actions.actionQueue.clear();

  battleActionQueue.actionQueue.insert(
    battleActionQueue.actionQueue.end(),
    std::make_move_iterator(p2Actions.actionQueue.begin()),
    std::make_move_iterator(p2Actions.actionQueue.end()));

  p2Actions.actionQueue.clear();
}
}  // namespace pokesim