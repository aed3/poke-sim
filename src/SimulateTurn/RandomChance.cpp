#include "RandomChance.hpp"

#include <Battle/Clone/Clone.hpp>
#include <Components/CalcDamage/CriticalHit.hpp>
#include <Components/CloneFromCloneTo.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/Probability.hpp>
#include <Components/RNGSeed.hpp>
#include <Components/RandomEventInputs.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Components/Tags/Current.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Damage.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Random.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <Utilities/RNG.hpp>
#include <Utilities/Tags.hpp>
#include <cstdint>
#include <optional>
#include <type_traits>

namespace pokesim {
namespace internal {
template <typename Type>
void updateProbability(Probability& currentProbability, Type percentChance) {
  currentProbability.val *= (types::probability)percentChance / 100.0F;
}

template <types::eventPossibilities POSSIBLE_EVENT_COUNT, typename RandomEventTag>
void updateProbabilityFromRandomEventChance(
  types::registry& registry, const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances, const Battle& battle) {
  Probability& probability = registry.get<Probability>(battle.val);

  if constexpr (std::is_same_v<RandomEventTag, tags::RandomEventA>) {
    updateProbability(probability, eventChances.chanceA());
  }
  else if constexpr (std::is_same_v<RandomEventTag, tags::RandomEventB>) {
    updateProbability(probability, eventChances.chanceB());
  }
  else if constexpr (std::is_same_v<RandomEventTag, tags::RandomEventC>) {
    updateProbability(probability, eventChances.chanceC());
  }
  else if constexpr (std::is_same_v<RandomEventTag, tags::RandomEventD>) {
    updateProbability(probability, eventChances.chanceD());
  }
  else if constexpr (std::is_same_v<RandomEventTag, tags::RandomEventE>) {
    updateProbability(probability, eventChances.chanceE());
  }
}

template <types::eventPossibilities POSSIBLE_EVENT_COUNT, typename RandomEventTag>
void viewUpdateProbabilityFromRandomEventChance(Simulation& simulation) {
  simulation.view<updateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, RandomEventTag>, Tags<RandomEventTag>>();
}

template <bool Reciprocal, typename RandomEventTag>
void updateProbabilityFromRandomBinaryChance(
  types::registry& registry, const RandomBinaryChance& eventChance, const Battle& battle) {
  Probability& probability = registry.get<Probability>(battle.val);

  if constexpr (std::is_same_v<RandomEventTag, tags::RandomEventCheckPassed>) {
    if constexpr (Reciprocal) {
      updateProbability(probability, eventChance.reciprocalPass());
    }
    else {
      updateProbability(probability, eventChance.pass());
    }
  }
  else if constexpr (std::is_same_v<RandomEventTag, tags::RandomEventCheckFailed>) {
    if constexpr (Reciprocal) {
      updateProbability(probability, eventChance.reciprocalFail());
    }
    else {
      updateProbability(probability, eventChance.fail());
    }
  }
}

void updateProbabilityFromRandomEqualChance(
  types::registry& registry, const Battle& battle, const RandomEventIndex&,
  types::eventPossibilities possibleEventCount) {
  Probability& probability = registry.get<Probability>(battle.val);

  updateProbability(probability, 100.0F / (float)possibleEventCount);
}

void updateProbabilityFromRandomEventCount(
  types::registry& registry, const RandomEventCount& eventChance, const Battle& battle) {
  Probability& probability = registry.get<Probability>(battle.val);

  updateProbability(probability, 100.0F / (float)eventChance.val);
}

template <types::eventPossibilities POSSIBLE_EVENT_COUNT>
void assignRandomEvent(
  types::handle handle, const Battle& battle, const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances) {
  auto [rngSeed, probability] = handle.registry()->get<RngSeed, Probability>(battle.val);
  types::percentChance rng = (types::percentChance)nextBoundedRandomValue(rngSeed, 100);

  if (rng <= eventChances.val[0]) {
    handle.emplace<tags::RandomEventA>();
    updateProbability(probability, eventChances.chanceA());
    return;
  }
  if (rng <= eventChances.val[1]) {
    handle.emplace<tags::RandomEventB>();
    updateProbability(probability, eventChances.chanceB());
    return;
  }
  if constexpr (POSSIBLE_EVENT_COUNT >= 3U) {
    if (rng <= eventChances.val[2]) {
      handle.emplace<tags::RandomEventC>();
      updateProbability(probability, eventChances.chanceC());
      return;
    }
  }

  if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
    if (rng <= eventChances.val[3]) {
      handle.emplace<tags::RandomEventD>();
      updateProbability(probability, eventChances.chanceD());
      return;
    }
  }

  if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
    if (rng <= eventChances.val[4]) {
      handle.emplace<tags::RandomEventE>();
      updateProbability(probability, eventChances.chanceE());
    }
  }
}

void assignRandomBinaryEvent(types::handle handle, const Battle& battle, const RandomBinaryChance& eventChance) {
  auto [rngSeed, probability] = handle.registry()->get<RngSeed, Probability>(battle.val);
  types::percentChance rng = (types::percentChance)nextBoundedRandomValue(rngSeed, 100);

  if (rng <= eventChance.pass()) {
    handle.emplace<tags::RandomEventCheckPassed>();
    updateProbability(probability, eventChance.pass());
  }
  else {
    handle.emplace<tags::RandomEventCheckFailed>();
    updateProbability(probability, eventChance.fail());
  }
}

void assignReciprocalRandomBinaryEvent(
  types::handle handle, const Battle& battle, const RandomBinaryChance& eventChance) {
  auto [rngSeed, probability] = handle.registry()->get<RngSeed, Probability>(battle.val);
  types::percentChance rng = (types::percentChance)nextBoundedRandomValue(rngSeed, eventChance.val);

  if (rng == 0) {
    handle.emplace<tags::RandomEventCheckPassed>();
    updateProbability(probability, eventChance.reciprocalPass());
  }
  else {
    handle.emplace<tags::RandomEventCheckFailed>();
    updateProbability(probability, eventChance.reciprocalFail());
  }
}

void assignRandomEqualChance(types::handle handle, const Battle& battle, types::eventPossibilities possibleEventCount) {
  auto [rngSeed, probability] = handle.registry()->get<RngSeed, Probability>(battle.val);
  types::percentChance rng = (types::percentChance)nextBoundedRandomValue(rngSeed, possibleEventCount);

  handle.emplace<RandomEventIndex>(rng);
  updateProbability(probability, 100.0F / (float)possibleEventCount);
}

void assignRandomEventCount(types::handle handle, const Battle& battle, const RandomEventCount& eventCount) {
  auto [rngSeed, probability] = handle.registry()->get<RngSeed, Probability>(battle.val);
  types::percentChance rng = (types::percentChance)nextBoundedRandomValue(rngSeed, eventCount.val);

  handle.emplace<RandomEventIndex>(rng);
  updateProbability(probability, 100.0F / (float)eventCount.val);
}

void assignIndexToClones(types::registry& registry, const std::vector<types::entity>& cloned, types::entity original) {
  registry.emplace<RandomEventIndex>(original, (types::eventPossibilities)0U);
  for (std::size_t index = 0; index < cloned.size(); index++) {
    ENTT_ASSERT(
      std::numeric_limits<types::eventPossibilities>::max() > index,
      "Number of clones shouldn't be greater than the number of possible events");
    registry.emplace<RandomEventIndex>(cloned[index], (types::eventPossibilities)(index + 1U));
  }
}

template <typename Stack, typename Random>
void placeChanceFromStack(types::handle battleHandle, Stack& stack) {
  if constexpr (!std::is_empty_v<Random>) {
    auto [eventChances, entity] = stack.val.back();
    battleHandle.registry()->emplace<Random>(entity, eventChances);
  }
  else {
    auto entity = stack.val.back();
    battleHandle.registry()->emplace<Random>(entity);
  }

  stack.val.pop_back();
  if (stack.val.empty()) {
    battleHandle.remove<Stack>();
  }
}

template <
  typename Random, typename RandomStack, auto AssignRandomEvents, typename UpdateProbabilities,
  typename... AssignRandomEventsTags, typename... AssignArgs>
void randomChanceEvent(
  Simulation& simulation, types::cloneIndex cloneCount, types::callback applyChoices,
  void (*assignClonesToEvents)(types::registry&, const std::vector<types::entity>&, types::entity),
  UpdateProbabilities updateProbabilities, const AssignArgs&... assignArgs) {
  if (simulation.battleFormat == BattleFormat::DOUBLES_BATTLE_FORMAT) {
    simulation.view<placeChanceFromStack<RandomStack, Random>>();
  }

  types::registry& registry = simulation.registry;
  if (simulation.simulateTurnOptions.makeBranchesOnRandomEvents()) {
    auto chanceEntityView = registry.view<Random>();
    if (chanceEntityView.empty()) {
      applyChoices(simulation);
      clearRandomChanceResult(simulation);
      return;
    }

    if constexpr (std::is_same_v<RandomEventCount, Random>) {
      entt::dense_map<types::eventPossibilities, std::pair<std::vector<types::entity>, std::vector<types::entity>>>
        entitiesByEventCount{};

      auto assignCloneTags =
        [&entitiesByEventCount](entt::entity chanceEntity, const Battle& battle, const RandomEventCount& eventCount) {
          entitiesByEventCount[eventCount.val].first.push_back(chanceEntity);
          entitiesByEventCount[eventCount.val].second.push_back(battle.val);
        };

      registry.view<Battle, RandomEventCount>().each(assignCloneTags);

      for (const auto& [eventCount, entities] : entitiesByEventCount) {
        const auto [chanceEntities, battleEntities] = entities;
        if (eventCount == 1U) {
          for (types::entity original : chanceEntities) {
            assignClonesToEvents(registry, {}, original);
          }
          continue;
        }

        registry.insert<tags::CloneFrom>(battleEntities.begin(), battleEntities.end());
        auto clonedEntityMap = clone(registry, eventCount - 1U);

        for (types::entity original : chanceEntities) {
          assignClonesToEvents(registry, clonedEntityMap[original], original);
        }
      }
    }
    else {
      auto assignCloneTags = [&registry](const Battle& battle, auto&&...) {
        registry.emplace<tags::CloneFrom>(battle.val);
      };
      registry.view<Battle, Random>().each(assignCloneTags);

      std::vector<types::entity> chanceEntities{chanceEntityView.begin(), chanceEntityView.end()};
      auto clonedEntityMap = clone(registry, cloneCount);

      for (types::entity original : chanceEntities) {
        assignClonesToEvents(registry, clonedEntityMap[original], original);
      }
    }

    applyChoices(simulation);
    updateProbabilities(simulation);
  }
  else {
    simulation.view<AssignRandomEvents, Tags<AssignRandomEventsTags...>>(assignArgs...);
    applyChoices(simulation);
    updateProbabilities(simulation);
  }

  registry.clear<Random>();
  clearRandomChanceResult(simulation);
  if (simulation.battleFormat == BattleFormat::DOUBLES_BATTLE_FORMAT && !registry.view<RandomStack>().empty()) {
    randomChanceEvent<Random, RandomStack, AssignRandomEvents, UpdateProbabilities, AssignRandomEventsTags...>(
      simulation,
      cloneCount,
      applyChoices,
      assignClonesToEvents,
      updateProbabilities,
      assignArgs...);
  }
}

template <bool Reciprocal>
void randomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities) {
  auto assignClonesToEvents =
    [](types::registry& registry, const std::vector<types::entity>& cloned, types::entity original) {
      registry.emplace<tags::RandomEventCheckPassed>(original);
      registry.emplace<tags::RandomEventCheckFailed>(cloned[0]);
    };

  auto defaultUpdateProbabilities = [](Simulation& sim) {
    sim.view<
      updateProbabilityFromRandomBinaryChance<Reciprocal, tags::RandomEventCheckPassed>,
      Tags<tags::RandomEventCheckPassed>>();
    sim.view<
      updateProbabilityFromRandomBinaryChance<Reciprocal, tags::RandomEventCheckFailed>,
      Tags<tags::RandomEventCheckFailed>>();
  };

  if constexpr (Reciprocal) {
    return randomChanceEvent<
      RandomBinaryChance,
      RandomBinaryChanceStack,
      assignReciprocalRandomBinaryEvent,
      types::callback>(
      simulation,
      1U,
      applyChoices,
      assignClonesToEvents,
      updateProbabilities.value_or(defaultUpdateProbabilities));
  }
  else {
    return randomChanceEvent<RandomBinaryChance, RandomBinaryChanceStack, assignRandomBinaryEvent, types::callback>(
      simulation,
      1U,
      applyChoices,
      assignClonesToEvents,
      updateProbabilities.value_or(defaultUpdateProbabilities));
  }
}
}  // namespace internal

template <types::eventPossibilities POSSIBLE_EVENT_COUNT>
void randomEventChances(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities) {
  static_assert(
    POSSIBLE_EVENT_COUNT >= 2U,
    "RandomEventChances should only be used for events with more than two options.");
  static_assert(POSSIBLE_EVENT_COUNT <= internal::MAX_TYPICAL_RANDOM_OPTIONS);

  auto assignClonesToEvents =
    [](types::registry& registry, const std::vector<types::entity>& cloned, types::entity original) {
      registry.emplace<tags::RandomEventA>(original);
      registry.emplace<tags::RandomEventB>(cloned[0]);
      if constexpr (POSSIBLE_EVENT_COUNT >= 3U) {
        registry.emplace<tags::RandomEventC>(cloned[1]);
      }
      if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
        registry.emplace<tags::RandomEventD>(cloned[2]);
      }
      if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
        registry.emplace<tags::RandomEventE>(cloned[3]);
      }
    };

  auto defaultUpdateProbabilities = [](Simulation& sim) {
    internal::viewUpdateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, tags::RandomEventA>(sim);
    internal::viewUpdateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, tags::RandomEventB>(sim);

    if constexpr (POSSIBLE_EVENT_COUNT >= 3U) {
      internal::viewUpdateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, tags::RandomEventC>(sim);
    }
    if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
      internal::viewUpdateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, tags::RandomEventD>(sim);
    }
    if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
      internal::viewUpdateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, tags::RandomEventE>(sim);
    }
  };

  return internal::randomChanceEvent<
    RandomEventChances<POSSIBLE_EVENT_COUNT>,
    RandomEventChancesStack<POSSIBLE_EVENT_COUNT>,
    internal::assignRandomEvent<POSSIBLE_EVENT_COUNT>,
    types::callback>(
    simulation,
    POSSIBLE_EVENT_COUNT - 1U,
    applyChoices,
    assignClonesToEvents,
    updateProbabilities.value_or(defaultUpdateProbabilities));
}

void randomEqualChance(
  Simulation& simulation, const types::eventPossibilities possibleEventCount, types::callback applyChoices,
  types::optionalCallback updateProbabilities) {
  auto defaultUpdateProbabilities = [possibleEventCount](Simulation& sim) {
    sim.view<internal::updateProbabilityFromRandomEqualChance>(possibleEventCount);
  };

  types::cloneIndex cloneCount =
    possibleEventCount > MechanicConstants::MAX_DAMAGE_ROLL_COUNT ? 0 : possibleEventCount - 1;

  if (updateProbabilities.has_value()) {
    return internal::randomChanceEvent<
      tags::RandomEqualChance,
      RandomEqualChanceStack,
      internal::assignRandomEqualChance,
      types::callback,
      tags::RandomEqualChance>(
      simulation,
      cloneCount,
      applyChoices,
      internal::assignIndexToClones,
      updateProbabilities.value(),
      possibleEventCount);
  }

  return internal::randomChanceEvent<
    tags::RandomEqualChance,
    RandomEqualChanceStack,
    internal::assignRandomEqualChance,
    decltype(defaultUpdateProbabilities),
    tags::RandomEqualChance>(
    simulation,
    cloneCount,
    applyChoices,
    internal::assignIndexToClones,
    defaultUpdateProbabilities,
    possibleEventCount);
}

void randomEventCount(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities) {
  auto defaultUpdateProbabilities = [](Simulation& sim) {
    sim.view<internal::updateProbabilityFromRandomEventCount>();
  };

  return internal::
    randomChanceEvent<RandomEventCount, RandomEventCountStack, internal::assignRandomEventCount, types::callback>(
      simulation,
      0U,
      applyChoices,
      internal::assignIndexToClones,
      updateProbabilities.value_or(defaultUpdateProbabilities));
}

void randomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities) {
  internal::randomBinaryChance<false>(simulation, applyChoices, updateProbabilities);
}

void reciprocalRandomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities) {
  internal::randomBinaryChance<true>(simulation, applyChoices, updateProbabilities);
}

void clearRandomChanceResult(Simulation& simulation) {
  simulation.registry.clear<tags::RandomEventA>();
  simulation.registry.clear<tags::RandomEventB>();
  simulation.registry.clear<tags::RandomEventC>();
  simulation.registry.clear<tags::RandomEventD>();
  simulation.registry.clear<tags::RandomEventE>();
  simulation.registry.clear<RandomEventIndex>();
}

template void randomEventChances<2U>(Simulation&, types::callback, types::optionalCallback);
template void randomEventChances<3U>(Simulation&, types::callback, types::optionalCallback);
template void randomEventChances<4U>(Simulation&, types::callback, types::optionalCallback);
template void randomEventChances<5U>(Simulation&, types::callback, types::optionalCallback);
}  // namespace pokesim