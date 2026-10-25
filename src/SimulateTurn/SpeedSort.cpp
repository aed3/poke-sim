#include "SpeedSort.hpp"

#include <Battle/Helpers/Helpers.hpp>
#include <Components/ActionQueue.hpp>
#include <Components/EntityHolders/ActiveWithUpdateEvents.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/RandomEventInputs.hpp>
#include <Components/SimulateTurn/SpeedTieIndexes.hpp>
#include <Components/Stats.hpp>
#include <Config/Require.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Types/Entity.hpp>
#include <Types/Indexes.hpp>
#include <Types/MaxSizedVector.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <Utilities/RNG.hpp>
#include <algorithm>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::internal::simulate_turn {
namespace {
template <typename List, typename IsFasterThan, typename IsSameSpeed>
void sortAndSetSpeedTies(
  types::handle handle, List& list, types::actionQueueIndex first, types::actionQueueIndex size,
  IsFasterThan isFasterThan, IsSameSpeed isSameSpeed) {
  POKESIM_REQUIRE(!list.empty(), "The action queue cannot be empty when sorting.");
  POKESIM_REQUIRE(first + size <= list.size(), "The last index would have been out of bounds.");

  // TODO(aed3): Test how different sorting algorithms affect speed
  std::sort(std::next(list.begin(), first), std::next(list.begin(), first + size), isFasterThan);

  SpeedTieIndexes speedTies;
  types::actionQueueIndex lastEqual = first, tieCount = 1U;

  for (types::actionQueueIndex i = first + 1U; i < first + size; i++) {
    if (isSameSpeed(list[i], list[i - 1U])) {
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

void sortActionQueueAndSetSpeedTies(
  types::handle handle, ActionQueue& actionQueue, types::actionQueueIndex first, types::actionQueueIndex size) {
  sortAndSetSpeedTies(
    handle,
    actionQueue.val,
    first,
    size,
    [](const ActionQueueItem& itemA, const ActionQueueItem& itemB) { return itemA.isFasterThan(itemB); },
    [](const ActionQueueItem& itemA, const ActionQueueItem& itemB) { return itemA.isSameSpeed(itemB); });
}

template <typename List>
void setSpeedTieOrder(List& list, const SpeedTieIndexes& speedTies, RandomEventIndex randomEventIndex) {
  const auto& speedTie = speedTies.val.back();
  shuffleShortList(list.val, speedTie.start, speedTie.length, randomEventIndex.val);
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

template <typename List>
void resolveSpeedTies(Simulation& simulation) {
  using Limit = Constants::ActivePokemon;
  types::activePokemonIndex speedTiesResolved = Limit::MIN;
  while (!simulation.hasNone<SpeedTieIndexes>()) {
    POKESIM_REQUIRE(
      speedTiesResolved <= Limit::MAX,
      "More speed ties were resolved than are possible in at least one battle.");

    simulation.view<setSpeedTieRandomEventCount>(simulation);
    randomEventCount(simulation, [](Simulation& sim) { sim.view<setSpeedTieOrder<List>>(); }, {});

    simulation.view<removeEmptySpeedTieIndexes>();
    speedTiesResolved++;
  }
}
}  // namespace

void speedSort(types::handle handle, ActionQueue& actionQueue) {
  if (actionQueue.val.size() <= 1U) {
    return;
  }

  sortActionQueueAndSetSpeedTies(handle, actionQueue, 0U, actionQueue.val.size());
}

void speedSortActiveWithUpdateEvents(types::handle handle, ActiveWithUpdateEvents& activeWithUpdateEvents) {
  if (activeWithUpdateEvents.val.size() <= 1U) {
    return;
  }

  types::registry& registry = *handle.registry();
  sortAndSetSpeedTies(
    handle,
    activeWithUpdateEvents.val,
    0U,
    activeWithUpdateEvents.val.size(),
    [&registry](types::entity entityA, types::entity entityB) {
      return registry.get<stat::EffectiveSpe>(entityA).val > registry.get<stat::EffectiveSpe>(entityB).val;
    },
    [&registry](types::entity entityA, types::entity entityB) {
      return registry.get<stat::EffectiveSpe>(entityA).val == registry.get<stat::EffectiveSpe>(entityB).val;
    });
}

void speedSortMidTurnSwitches(types::handle handle, ActionQueue& actionQueue) {
  types::activePokemonIndex midTurnSwitches = 0U;
  for (; midTurnSwitches < Constants::ActivePokemon::MAX && midTurnSwitches < actionQueue.val.size();
       midTurnSwitches++) {
    if (actionQueue.val[midTurnSwitches].order != ActionOrder::MID_TURN_SWITCH) {
      break;
    }
  }

  if (midTurnSwitches <= 1U) {
    return;
  }

  sortActionQueueAndSetSpeedTies(handle, actionQueue, 0U, midTurnSwitches);
}

void resolveActionQueueSpeedTies(Simulation& simulation) {
  resolveSpeedTies<ActionQueue>(simulation);
}

void resolveUpdateEventSpeedTies(Simulation& simulation) {
  resolveSpeedTies<ActiveWithUpdateEvents>(simulation);
}
}  // namespace pokesim::internal::simulate_turn
