#include "ManageActionQueue.hpp"

#include <Components/ActionQueue.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/RecycledEntities.hpp>
#include <Components/Names/ItemNames.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Config/Require.hpp>
#include <Types/Decisions.hpp>
#include <Types/Enums/ActionOrder.hpp>
#include <Types/MaxSizedVector.hpp>
#include <Types/Registry.hpp>
#include <Types/Variant.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::internal::simulate_turn {
void addBeforeTurnAction(ActionQueue& actionQueue) {
  actionQueue.val.push_back({ActionOrder::BEFORE_TURN});
}

void addResidualAction(ActionQueue& actionQueue) {
  actionQueue.val.push_back({ActionOrder::RESIDUAL});
}

void setCurrentAction(types::handle battleHandle, ActionQueue& actionQueue, RecycledAction action) {
  types::registry& registry = *battleHandle.registry();

  if (actionQueue.val.empty()) {
    return;
  }

  ActionQueueItem nextActon = actionQueue.val.front();
  registry.emplace<action::tags::Current>(action.val);

  switch (nextActon.order) {
    case ActionOrder::MID_TURN_SWITCH:
    case ActionOrder::SWITCH:          {
      battleHandle.emplace<action::tags::Switch>();
      const SwitchDecision& decision = nextActon.decision.get<SwitchDecision>();
      registry.emplace<SourceSlotName>(action.val, decision.sourceSlot);
      registry.emplace<TargetSlotName>(action.val, decision.targetSlot);
      break;
    }
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
}  // namespace pokesim::internal::simulate_turn
