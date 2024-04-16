#include "Actions.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Components/Decisions.hpp>
#include <Components/EntityHolders/ActionQueue.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/SimulateTurn/ActionNames.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/SimulateTurn/SpeedTieIndexes.hpp>
#include <Components/SimulateTurn/TeamAction.hpp>
#include <Components/SpeedSort.hpp>
#include <Components/Stats.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/ActionOrder.hpp>
#include <Types/State.hpp>
#include <Utilities/Variant.hpp>
#include <algorithm>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <vector>

namespace pokesim {
void resolveDecision(types::handle sideHandle, const SideDecision& sideDecision) {
  ENTT_ASSERT(sideDecision.sideId != PlayerSideId::NONE, "Decisions must be assigned to a player");
  ENTT_ASSERT(!sideDecision.decisions.valueless_by_exception(), "Decisions must be non-empty");
  types::registry& registry = *sideHandle.registry();

  ActionQueue& battleActionQueue = sideHandle.registry()->get<ActionQueue>(sideHandle.get<Battle>().battle);

  if (sideDecision.decisions.holds<types::slotDecisions>()) {
    const auto& decisions = sideDecision.decisions.get<types::slotDecisions>();

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
        speedSort.priority = 0;                // TODO (aed3): Move priority + modify priority
        speedSort.fractionalPriority = false;  // TODO (aed3): get fractionalPriority
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

      battleActionQueue.actionQueue.push_back(actionHandle.entity());
    }
  }
  else if (sideDecision.decisions.holds<types::teamOrder>()) {
    const auto& teamOrder = sideDecision.decisions.get<types::teamOrder>();

    ENTT_ASSERT(
      sideHandle.get<Team>().team.size() == teamOrder.size(),
      "Must pick a placement for each Pokemon on the team");
    types::handle actionHandle = {registry, registry.create()};

    actionHandle.emplace<action::Team>(teamOrder);

    battleActionQueue.actionQueue.push_back(actionHandle.entity());
  }
}

void speedSort(types::handle handle, ActionQueue& actionQueue) {
  std::vector<types::entity>& entityList = actionQueue.actionQueue;

  if (entityList.size() == 1) return;
  const types::registry* registry = handle.registry();

  std::vector<std::pair<SpeedSort, types::entity>> speedSortList;
  speedSortList.reserve(entityList.size());

  for (types::entity entity : entityList) {
    speedSortList.push_back({registry->get<SpeedSort>(entity), entity});
  }

  // TODO (aed3): Test how different sorting algorithms effect speed
  std::sort(speedSortList.begin(), speedSortList.end(), [](const auto& pairA, const auto& pairB) {
    if (pairA.first.order != pairB.first.order) {
      return pairA.first.order < pairB.first.order;
    }

    if (pairA.first.priority != pairB.first.priority) {
      return pairB.first.priority < pairA.first.priority;
    }

    if (pairA.first.fractionalPriority != pairB.first.fractionalPriority) {
      return pairB.first.fractionalPriority;
    }

    if (pairA.first.speed != pairB.first.speed) {
      return pairB.first.speed < pairA.first.speed;
    }

    return false;
  });

  SpeedTieIndexes speedTies;
  std::size_t lastEqual = 0, tieCount = 1;

  auto speedSortEqual = [](const SpeedSort& speedSortA, const SpeedSort& speedSortB) {
    return speedSortA.order == speedSortB.order && speedSortA.priority == speedSortB.priority &&
           speedSortA.speed == speedSortB.speed && speedSortA.fractionalPriority == speedSortB.fractionalPriority;
  };

  for (std::size_t i = 0; i < speedSortList.size(); i++) {
    entityList[i] = speedSortList[i].second;

    if (i > 0 && speedSortEqual(speedSortList[i].first, speedSortList[i - 1].first)) {
      tieCount++;
    }
    else {
      if (tieCount > 1) {
        speedTies.spans.push_back({lastEqual, tieCount});
      }
      lastEqual = i;
      tieCount = 1;
    }
  }

  if (tieCount > 1) {
    speedTies.spans.push_back({lastEqual, tieCount});
  }

  if (!speedTies.spans.empty()) {
    handle.emplace<SpeedTieIndexes>(speedTies);
  }
}
}  // namespace pokesim