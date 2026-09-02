#include "ManageActionQueue.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Components/ActionQueue.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/RecycledEntities.hpp>
#include <Components/Names/ItemNames.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SourceSlotName.hpp>
#include <Components/Names/TargetSlotName.hpp>
#include <Components/RandomEventInputs.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/SimulateTurn/SpeedTieIndexes.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Config/Require.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Types/Decisions.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/ActionOrder.hpp>
#include <Types/Indexes.hpp>
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
void sortAndSetSpeedTies(
  types::handle handle, ActionQueue& actionQueue, types::actionQueueIndex first, types::actionQueueIndex size) {
  POKESIM_REQUIRE(first + size <= actionQueue.val.size(), "The last index would have been out of bounds.");
  auto& actionQueueItems = actionQueue.val;

  // TODO(aed3): Test how different sorting algorithms affect speed
  std::sort(
    &actionQueueItems[first],
    &actionQueueItems[first + size],
    [](const ActionQueueItem& itemA, const ActionQueueItem& itemB) { return itemA.isFasterThan(itemB); });

  SpeedTieIndexes speedTies;
  types::actionQueueIndex lastEqual = first, tieCount = 1U;

  for (types::actionQueueIndex i = first + 1U; i < first + size; i++) {
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

void setBattleOnSpeedTyingEntities(types::handle battleHandle, const SpeedTieIndexes&) {
  battleHandle.emplace<Battle>(battleHandle.entity());
}

void setSpeedTieRandomEventCount(types::handle handle, const SpeedTieIndexes& speedTies, const Simulation& simulation) {
  POKESIM_REQUIRE(
    !speedTies.val.empty(),
    "SpeedTieIndexes shouldn't exist if there are no speed ties left to resolve.");

  types::activePokemonIndex length = speedTies.val.back().length;
  if constexpr (Constants::ActivePokemon::MAX > 4U) {
    POKESIM_REQUIRE(
      length <= 4U,
      "The below array doesn't contain the precomputed factorial value for the amount of Pokemon speed tying. Update "
      "the array and this assert with the correct values.");
  }
  static constexpr std::array<types::eventPossibilities, Constants::ActivePokemon::MAX - 1U> factorials = {2U, 6U, 24U};

  types::eventPossibilities eventPossibilities = factorials[length - 2U];
  setRandomEventCountsFromPossibilities(handle, Battle{handle.entity()}, simulation, eventPossibilities, false);
}

void removeEmptySpeedTieIndexes(types::handle handle, SpeedTieIndexes& speedTies) {
  speedTies.val.pop_back();
  if (speedTies.val.empty()) {
    handle.remove<SpeedTieIndexes>();
  }
}
}  // namespace

void speedSort(types::handle handle, ActionQueue& actionQueue) {
  if (actionQueue.val.size() == 1U) {
    return;
  }

  sortAndSetSpeedTies(handle, actionQueue, 0U, actionQueue.val.size());
}

void speedSortMidTurnSwitches(types::handle handle, ActionQueue& actionQueue) {
  types::activePokemonIndex midTurnSwitches = 0U;
  for (; midTurnSwitches < Constants::ActivePokemon::MAX; midTurnSwitches++) {
    if (actionQueue.val[midTurnSwitches].order != ActionOrder::MID_TURN_SWITCH) {
      break;
    }
  }

  if (midTurnSwitches <= 1U) {
    return;
  }

  sortAndSetSpeedTies(handle, actionQueue, 0U, midTurnSwitches);
}

void setSpeedTieOrder(ActionQueue& actionQueue, const SpeedTieIndexes& speedTies, RandomEventIndex randomEventIndex) {
  const auto& speedTie = speedTies.val.back();

  // This loop shuffles a list based on one random number where each possible shuffling has an equal chance. The main
  // idea is to choose where each action should be placed in that new ordering by using the ranges the random number
  // belongs to. For example, let's say there are 3 actions that speed tied, the random number (randomEventIndex) is 2,
  // and we want to find where the third action in the original ordering should move to. There are 6 different ways to
  // order those actions (3 factorial = 6), meaning the randomEventIndex could include any value from 0 to 5. The code
  // below first separates the 0 to 5 range into 3 equally sized sections (so 2 values per section) and then calculates
  // that the randomEventIndex of 2 is in the second section. That section's index is used as where the third action
  // will go as there will be as many sections as there are places to move that action.
  types::eventPossibilities valuesPerSection = 1U;
  for (types::actionQueueIndex sections = 1U; sections < speedTie.length; sections++) {
    types::eventPossibilities totalValues = (sections + 1U) * valuesPerSection;
    types::eventPossibilities eventIndexInAllValues = randomEventIndex.val % totalValues;
    types::eventPossibilities sectionOfEventIndex = eventIndexInAllValues / valuesPerSection;

    types::activePokemonIndex oldIndex = speedTie.start + sections;
    types::activePokemonIndex newIndex = speedTie.start + sectionOfEventIndex;

    POKESIM_REQUIRE(
      newIndex < speedTie.start + speedTie.length,
      "newIndex would be out of bounds for the current speed tie.");

    std::swap(actionQueue.val[oldIndex], actionQueue.val[newIndex]);
    valuesPerSection = totalValues;
  }
}

void resolveSpeedTies(Simulation& simulation) {
  using Limit = Constants::ActivePokemon;
  types::activePokemonIndex speedTiesResolved = Limit::MIN;
  while (!simulation.registry.view<SpeedTieIndexes>().empty()) {
    POKESIM_REQUIRE(
      speedTiesResolved <= Limit::MAX,
      "More speed ties were resolved than are possible in at least one battle.");
    // The random chance functions require Battle components and this is the only case of battle entities using random
    // chance functions.
    if (!speedTiesResolved) {
      simulation.view<setBattleOnSpeedTyingEntities>();
    }

    simulation.view<setSpeedTieRandomEventCount>(simulation);
    randomEventCount(simulation, [](Simulation& sim) { sim.view<setSpeedTieOrder>(); }, {});

    simulation.view<removeEmptySpeedTieIndexes>();
    speedTiesResolved++;
  }

  if (speedTiesResolved != Limit::MIN) {
    simulation.removeFromEntities<Battle, pokesim::tags::Battle>();
  }
}

void addBeforeTurnAction(ActionQueue& actionQueue) {
  actionQueue.val.push_back({ActionOrder::BEFORE_TURN});
}

void addResidualAction(ActionQueue& actionQueue) {
  actionQueue.val.push_back({ActionOrder::RESIDUAL});
}

void setCurrentAction(types::handle battleHandle, ActionQueue& actionQueue, RecycledAction action) {
  types::registry& registry = *battleHandle.registry();

  if (actionQueue.val.empty()) {
    battleHandle.remove<pokesim::tags::BattleMidTurn>();
    return;
  }

  ActionQueueItem nextActon = actionQueue.val.front();
  registry.emplace<action::tags::Current>(action.val);

  switch (nextActon.order) {
    case ActionOrder::SWITCH: {
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

void clearActionQueue(types::handle handle, ActionQueue& actionQueue) {
  handle.remove<pokesim::tags::BattleMidTurn>();
  actionQueue.val.clear();
}
}  // namespace pokesim::internal::simulate_turn
