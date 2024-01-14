#include "ResolveDecision.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/SimulateTurn/ActionNames.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/SimulateTurn/TeamAction.hpp>
#include <Components/SpeedSort.hpp>
#include <Components/Stats.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/ActionOrder.hpp>
#include <Types/State.hpp>
#include <Types/Utilities/variant.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

#include "Decisions.hpp"

namespace pokesim {
void resolveDecision(types::handle sideHandle, const SideDecision& sideDecision, ActionQueue& sideActionQueue) {
  ENTT_ASSERT(sideDecision.sideId != PlayerSideId::NONE, "Decisions must be assigned to a player");
  ENTT_ASSERT(!sideDecision.decisions.valueless_by_exception(), "Decisions must be non-empty");
  types::registry& registry = *sideHandle.registry();

  if (sideDecision.decisions.holds<types::SideSlots<SlotDecision>>()) {
    const auto& decisions = sideDecision.decisions.get<types::SideSlots<SlotDecision>>();

    for (const SlotDecision& decision : decisions) {
      ENTT_ASSERT(decision.sourceSlot != Slot::NONE, "Source slot must be assigned");
      if (sideDecision.sideId == PlayerSideId::P1) {
        ENTT_ASSERT(
          (decision.sourceSlot == Slot::P1A || decision.sourceSlot == Slot::P1B),
          "Source must be from a player 1 in battle slot");
      }
      else {
        ENTT_ASSERT(
          (decision.sourceSlot == Slot::P2A || decision.sourceSlot == Slot::P2B),
          "Source must be from a player 2 in battle slot");
      }

      ENTT_ASSERT(decision.targetSlot != Slot::NONE, "Target slot must be assigned");
      ENTT_ASSERT(
        !(decision.moveChoice.has_value() && decision.itemChoice.has_value()),
        "Decisions can't have a move and an item choice");

      types::handle actionHandle = {registry, registry.create()};
      actionHandle.emplace<SourceSlotName>(decision.sourceSlot);
      actionHandle.emplace<TargetSlotName>(decision.targetSlot);

      SpeedSort speedSort;
      types::entity sourceEntity = slotToEntity(registry, sideHandle.entity(), decision.sourceSlot);

      speedSort.speed = registry.get<stat::Spe>(sourceEntity).stat;  // TODO (aed3): getActionSpeed

      if (decision.moveChoice.has_value()) {
        actionHandle.emplace<action::Move>(decision.moveChoice.value());

        speedSort.order = ActionOrder::MOVE;
        speedSort.priority = 0;            // TODO (aed3): Move priority + modify priority
        speedSort.fractionalPriority = 0;  // TODO (aed3): get fractionalPriority
      }
      else if (decision.itemChoice.has_value()) {
        actionHandle.emplace<action::Item>(decision.itemChoice.value());
        speedSort.order = ActionOrder::ITEM;
      }
      else {
        actionHandle.emplace<action::tags::Switch>();
        speedSort.order = ActionOrder::SWITCH;
      }

      actionHandle.emplace<SpeedSort>(speedSort);

      sideActionQueue.actionQueue.push_back(actionHandle.entity());
    }
  }
  else if (sideDecision.decisions.holds<types::TeamPositions<types::TeamPositionIndex>>()) {
    const auto& teamOrder = sideDecision.decisions.get<types::TeamPositions<types::TeamPositionIndex>>();

    ENTT_ASSERT(
      sideHandle.get<Team>().team.size() == teamOrder.size(),
      "Must pick a placement for each Pokemon on the team");
    types::handle actionHandle = {registry, registry.create()};

    actionHandle.emplace<action::Team>(teamOrder);

    sideActionQueue.actionQueue.push_back(actionHandle.entity());
  }
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