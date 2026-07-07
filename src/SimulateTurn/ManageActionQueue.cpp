#include "ManageActionQueue.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Components/ActionQueue.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/RecycledEntities.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/Names/ItemNames.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/SideDecisions.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/SimulateTurn/SpeedTieIndexes.hpp>
#include <Components/SimulateTurn/TeamAction.hpp>
#include <Components/Stats.hpp>
#include <Components/Turn.hpp>
#include <Config/Require.hpp>
#include <Types/Decisions.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/ActionOrder.hpp>
#include <Types/MaxSizedVector.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <Types/Variant.hpp>
#include <algorithm>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <vector>

namespace pokesim::internal::simulate_turn {
namespace {
template <typename Decision>
void resolveSlotDecision(types::handle sideHandle, const types::slotDecision& slotDecision, ActionQueue& actionQueue) {
  if (!slotDecision.holds<Decision>()) {
    return;
  }

  types::registry& registry = *sideHandle.registry();
  const auto& decision = slotDecision.get<Decision>();

  POKESIM_REQUIRE(decision.sourceSlot != Slot::NONE, "Source slot must be assigned.");
  POKESIM_REQUIRE(decision.targetSlot != Slot::NONE, "Target slot must be assigned.");

  ActionQueueItem actionQueueItem;
  actionQueueItem.decision = decision;

  types::entity sourceEntity = slotToPokemonEntity(registry, sideHandle.entity(), decision.sourceSlot);
  actionQueueItem.speed = registry.get<stat::EffectiveSpe>(sourceEntity).val;

  if constexpr (std::is_base_of_v<MoveDecision, Decision>) {
    actionQueueItem.order = ActionOrder::MOVE;
    actionQueueItem.priority = Constants::MovePriority::DEFAULT;  // TODO (aed3): Move priority + modify priority
    actionQueueItem.fractionalPriority = false;                   // TODO (aed3): get fractionalPriority

    if constexpr (!std::is_same_v<MoveDecision, Decision>) {
      POKESIM_REQUIRE_FAIL(std::string(entt::type_name<Decision>().value()) + " is not yet supported.");
    }
  }
  else if constexpr (std::is_same_v<SwitchDecision, Decision>) {
    actionQueueItem.order = ActionOrder::SWITCH;
  }
  else if constexpr (std::is_same_v<ItemDecision, Decision>) {
    actionQueueItem.order = ActionOrder::ITEM;
  }
  else {
    POKESIM_REQUIRE_FAIL(std::string(entt::type_name<Decision>().value()) + " is not yet supported.");
  }

  actionQueue.val.push_back(actionQueueItem);
}

void resolveSlotDecisions(types::handle sideHandle, const types::slotDecisions& decisions, ActionQueue& actionQueue) {
  for (const types::slotDecision& decision : decisions) {
    resolveSlotDecision<MoveDecision>(sideHandle, decision, actionQueue);
    resolveSlotDecision<MegaEvolveAndMoveDecision>(sideHandle, decision, actionQueue);
    resolveSlotDecision<ZMoveDecision>(sideHandle, decision, actionQueue);
    resolveSlotDecision<DynamaxAndMoveDecision>(sideHandle, decision, actionQueue);
    resolveSlotDecision<TerastallizeAndMoveDecision>(sideHandle, decision, actionQueue);
    resolveSlotDecision<SwitchDecision>(sideHandle, decision, actionQueue);
    resolveSlotDecision<ItemDecision>(sideHandle, decision, actionQueue);
  }
}

void resolveTeamDecision(types::registry&, const types::teamOrder&, ActionQueue&) {}
}  // namespace

void resolveDecision(types::handle sideHandle, const SideDecision& sideDecision) {
  POKESIM_REQUIRE(sideDecision.sideId != PlayerSideId::NONE, "Decisions must be assigned to a player.");
  POKESIM_REQUIRE(!sideDecision.decisions.valueless_by_exception(), "Decisions must be non-empty.");

  const Battle& battle = sideHandle.get<Battle>();
  types::registry& registry = *sideHandle.registry();
  ActionQueue& actionQueue = registry.get<ActionQueue>(battle.val);

  if (sideDecision.decisions.holds<types::slotDecisions>()) {
    POKESIM_REQUIRE(
      registry.get<Turn>(battle.val).val != Constants::TurnCount::MIN,
      "Slot decisions only have an effect after a battle starts. Make sure to pass a `teamOrder` decision at the start "
      "of a battle (aka team preview).");
    const auto& decisions = sideDecision.decisions.get<types::slotDecisions>();

#ifdef POKESIM_DEBUG_CHECK_UTILITIES
    for (const auto& decision : decisions) {
      if (sideDecision.sideId == PlayerSideId::P1) {
        POKESIM_REQUIRE(
          (decision.sourceSlot() == Slot::P1A || decision.sourceSlot() == Slot::P1B),
          "Source must be from a player 1 in battle slot.");
      }
      else {
        POKESIM_REQUIRE(
          (decision.sourceSlot() == Slot::P2A || decision.sourceSlot() == Slot::P2B),
          "Source must be from a player 2 in battle slot.");
      }
    }
#endif

    resolveSlotDecisions(sideHandle, decisions, actionQueue);
  }
  else if (sideDecision.decisions.holds<types::teamOrder>()) {
    POKESIM_REQUIRE(
      registry.get<Turn>(battle.val).val == Constants::TurnCount::MIN,
      "Team order decisions only have an effect at the start of a battle (aka team preview). Make sure to pass a "
      "`slotDecisions` decision for battles in progress.");
    const auto& teamOrder = sideDecision.decisions.get<types::teamOrder>();

    POKESIM_REQUIRE(
      sideHandle.get<Team>().val.size() == teamOrder.size(),
      "Must pick a placement for each Pokemon on the team.");
    resolveTeamDecision(*sideHandle.registry(), teamOrder, actionQueue);
  }
  else {
    POKESIM_REQUIRE_FAIL(
      "Decision kind of index " + std::to_string(sideDecision.decisions.index()) + "not implemented yet.");
  }
}

void speedSort(types::handle handle, ActionQueue& actionQueue) {
  auto& actionQueueItems = actionQueue.val;

  if (actionQueueItems.size() == 1U) {
    return;
  }

  // TODO(aed3): Test how different sorting algorithms affect speed
  std::sort(
    actionQueueItems.begin(),
    actionQueueItems.end(),
    [](const ActionQueueItem& itemA, const ActionQueueItem& itemB) { return itemA.isFasterThan(itemB); });

  SpeedTieIndexes speedTies;
  types::activePokemonIndex lastEqual = 0U, tieCount = 1U;

  for (types::activePokemonIndex i = 1U; i < actionQueueItems.size(); i++) {
    if (actionQueueItems[i].isSameSpeed(actionQueueItems[i - 1U])) {
      tieCount++;
    }
    else {
      if (tieCount > 1U) {
        speedTies.val.push_back({lastEqual, tieCount});
      }
      lastEqual = i;
      tieCount = 1U;
    }
  }

  if (tieCount > 1U) {
    speedTies.val.push_back({lastEqual, tieCount});
  }

  if (!speedTies.val.empty()) {
    handle.emplace<SpeedTieIndexes>(speedTies);
  }
}

void addBeforeTurnAction(ActionQueue& actionQueue) {
  actionQueue.val.push_back({ActionOrder::BEFORE_TURN});
}

void addResidualAction(ActionQueue& actionQueue) {
  actionQueue.val.push_back({ActionOrder::RESIDUAL});
}

void setCurrentAction(types::handle battleHandle, ActionQueue& actionQueue, RecycledAction& action) {
  types::registry& registry = *battleHandle.registry();

  if (actionQueue.val.empty()) return;

  ActionQueueItem nextActon = actionQueue.val.front();
  registry.emplace<action::tags::Current>(action.val);

  switch (nextActon.order) {
    case ActionOrder::MOVE: {
      battleHandle.emplace<action::tags::Move>();
      const MoveDecision& decision = nextActon.decision.get<MoveDecision>();
      registry.emplace<SourceSlotName>(action.val, decision.sourceSlot);
      registry.emplace<TargetSlotName>(action.val, decision.targetSlot);
      registry.emplace<MoveName>(action.val, decision.move);
      break;
    }
    case ActionOrder::RESIDUAL: {
      battleHandle.emplace<action::tags::Residual>();
      break;
    }
    case ActionOrder::BEFORE_TURN: {
      battleHandle.emplace<action::tags::BeforeTurn>();
      break;
    }
    default: {
      POKESIM_REQUIRE_FAIL("Action kind not implemented yet.");
      break;
    }
  }

  actionQueue.val.erase(actionQueue.val.begin());

  battleHandle.emplace<CurrentAction>(action.val);
}

void clearActionQueue(ActionQueue& actionQueue) {
  actionQueue.val.clear();
}
}  // namespace pokesim::internal::simulate_turn
