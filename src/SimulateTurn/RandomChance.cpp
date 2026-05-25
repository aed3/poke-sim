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
#include <Config/Require.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Damage.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Random.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <Utilities/RNG.hpp>
#include <Utilities/Tags.hpp>
#include <optional>
#include <type_traits>

namespace pokesim::internal {
namespace {
bool constexpr useChanceStack(const Simulation& simulation) {
  return simulation.isBattleFormat(BattleFormat::DOUBLES) &&
         simulation.simulateTurnOptions.getMakeBranchesOnRandomEvents();
}

void updateProbability(Probability& currentProbability, types::probability eventProbability) {
  currentProbability.val *= eventProbability;
}

template <types::eventPossibilities POSSIBLE_EVENT_COUNT, typename RandomEventTag>
void updateProbabilityFromRandomEventChance(
  types::registry& registry, const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances, const Battle& battle) {
  static constexpr bool isEventA = std::is_same_v<RandomEventTag, tags::RandomEventA>;
  static constexpr bool isEventB = std::is_same_v<RandomEventTag, tags::RandomEventB>;
  static constexpr bool isEventC = std::is_same_v<RandomEventTag, tags::RandomEventC>;
  static constexpr bool isEventD = std::is_same_v<RandomEventTag, tags::RandomEventD>;
  static constexpr bool isEventE = std::is_same_v<RandomEventTag, tags::RandomEventE>;

  static_assert(
    isEventA || isEventB || isEventC || isEventD || isEventE,
    "Using a type that isn't a valid random event type.");

  Probability& probability = registry.get<Probability>(battle.val);

  if constexpr (isEventA) {
    updateProbability(probability, eventChances.probabilityA());
  }
  else if constexpr (isEventB) {
    updateProbability(probability, eventChances.probabilityB());
  }
  else if constexpr (isEventC) {
    updateProbability(probability, eventChances.probabilityC());
  }
  else if constexpr (isEventD) {
    updateProbability(probability, eventChances.probabilityD());
  }
  else if constexpr (isEventE) {
    updateProbability(probability, eventChances.probabilityE());
  }
}

template <types::eventPossibilities POSSIBLE_EVENT_COUNT, typename RandomEventTag>
void viewUpdateProbabilityFromRandomEventChance(Simulation& simulation) {
  simulation.view<updateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, RandomEventTag>, Tags<RandomEventTag>>();
}

template <typename RandomEventTag>
void updateProbabilityFromRandomBinaryChance(
  types::registry& registry, const RandomBinaryProbability& eventChance, const Battle& battle) {
  static constexpr bool hasEventPassed = std::is_same_v<RandomEventTag, tags::RandomEventCheckPassed>;
  static constexpr bool hasEventFailed = std::is_same_v<RandomEventTag, tags::RandomEventCheckFailed>;

  static_assert(hasEventPassed || hasEventFailed, "Using a type that isn't a valid random binary chance outcome type.");

  Probability& probability = registry.get<Probability>(battle.val);

  if constexpr (hasEventPassed) {
    updateProbability(probability, eventChance.pass());
  }
  else if constexpr (hasEventFailed) {
    updateProbability(probability, eventChance.fail());
  }
}

void updateProbabilityFromRandomEqualChance(
  types::registry& registry, const Battle& battle, const RandomEventIndex&,
  types::eventPossibilities possibleEventCount) {
  Probability& probability = registry.get<Probability>(battle.val);

  updateProbability(probability, MechanicConstants::Probability::MAX / (types::probability)possibleEventCount);
}

void updateProbabilityFromRandomEventCount(
  types::registry& registry, const RandomEventCount& eventChance, const Battle& battle) {
  Probability& probability = registry.get<Probability>(battle.val);

  updateProbability(probability, eventChance.probability());
}

template <types::eventPossibilities POSSIBLE_EVENT_COUNT>
void assignRandomEvent(
  types::handle handle, const Battle& battle, const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances) {
  RngSeed& rngSeed = handle.registry()->get<RngSeed>(battle.val);
  types::percentChance rng =
    (types::percentChance)nextBoundedRandomValue(rngSeed, MechanicConstants::PercentChance::MAX);

  if (rng <= eventChances.val[0]) {
    handle.emplace<tags::RandomEventA>();
    return;
  }
  if (rng <= eventChances.val[1]) {
    handle.emplace<tags::RandomEventB>();
    return;
  }

  if constexpr (POSSIBLE_EVENT_COUNT >= 3U) {
    if (rng <= eventChances.val[2]) {
      handle.emplace<tags::RandomEventC>();
      return;
    }
  }

  if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
    if (rng <= eventChances.val[3]) {
      handle.emplace<tags::RandomEventD>();
      return;
    }
  }

  if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
    if (rng <= eventChances.val[4]) {
      handle.emplace<tags::RandomEventE>();
    }
  }
}

void assignRandomBinaryEvent(types::handle handle, const Battle& battle, const RandomBinaryProbability& eventChance) {
  RngSeed& rngSeed = handle.registry()->get<RngSeed>(battle.val);
  types::probability rng = nextRandomValue(rngSeed) / (types::probability)std::numeric_limits<types::rngResult>::max();

  if (rng <= eventChance.pass()) {
    handle.emplace<tags::RandomEventCheckPassed>();
  }
  else {
    handle.emplace<tags::RandomEventCheckFailed>();
  }
}

void assignRandomEqualChance(types::handle handle, const Battle& battle, types::eventPossibilities possibleEventCount) {
  RngSeed& rngSeed = handle.registry()->get<RngSeed>(battle.val);
  types::eventPossibilities rng = (types::eventPossibilities)nextBoundedRandomValue(rngSeed, possibleEventCount);

  handle.emplace<RandomEventIndex>(rng);
}

void assignRandomEventCount(types::handle handle, const Battle& battle, const RandomEventCount& eventCount) {
  RngSeed& rngSeed = handle.registry()->get<RngSeed>(battle.val);
  types::eventPossibilities rng = (types::eventPossibilities)nextBoundedRandomValue(rngSeed, eventCount.val);

  handle.emplace<RandomEventIndex>(rng);
}

void assignIndexToClones(
  types::registry& registry, const types::ClonedEntityMap& clonedEntityMap, const types::entityVector& originals) {
  for (types::entity original : originals) {
    registry.emplace<RandomEventIndex>(original, (types::eventPossibilities)0U);

    const auto clonedPointer = clonedEntityMap.find(original);
    if (clonedPointer == clonedEntityMap.end()) continue;
    const auto& cloned = clonedPointer->second;

    for (types::entityIndex index = 0U; index < cloned.size(); index++) {
      POKESIM_REQUIRE(
        std::numeric_limits<types::eventPossibilities>::max() > index,
        "Number of clones shouldn't be greater than the number of possible events.");
      registry.emplace<RandomEventIndex>(cloned[index], (types::eventPossibilities)(index + 1U));
    }
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

void clearRandomChanceResult(Simulation& simulation) {
  simulation.registry.clear<tags::RandomEventA>();
  simulation.registry.clear<tags::RandomEventB>();
  simulation.registry.clear<tags::RandomEventC>();
  simulation.registry.clear<tags::RandomEventD>();
  simulation.registry.clear<tags::RandomEventE>();
  simulation.registry.clear<RandomEventIndex>();
}

template <
  typename Random, typename RandomStack, auto AssignRandomEvents, typename UpdateProbabilities,
  typename... AssignRandomEventsTags, typename... AssignArgs>
void randomChanceEvent(
  Simulation& simulation, types::entityIndex cloneCount, types::callback applyChoices,
  void (*assignClonesToEvents)(types::registry&, const types::ClonedEntityMap&, const types::entityVector&),
  UpdateProbabilities updateProbabilities, const AssignArgs&... assignArgs) {
  if (simulation.isBattleFormat(BattleFormat::DOUBLES)) {
    simulation.view<placeChanceFromStack<RandomStack, Random>>();
  }

  types::registry& registry = simulation.registry;
  if (simulation.simulateTurnOptions.getMakeBranchesOnRandomEvents()) {
    auto chanceEntityView = registry.view<Random>();
    if (chanceEntityView.empty()) {
      applyChoices(simulation);
      clearRandomChanceResult(simulation);
      return;
    }

    if constexpr (std::is_same_v<RandomEventCount, Random>) {
      entt::dense_map<types::eventPossibilities, std::pair<types::entityVector, types::entityVector>>
        entitiesByEventCount{};

      auto collectEntityEventCounts =
        [&entitiesByEventCount](types::entity chanceEntity, const Battle& battle, const RandomEventCount& eventCount) {
          entitiesByEventCount[eventCount.val].first.push_back(chanceEntity);
          entitiesByEventCount[eventCount.val].second.push_back(battle.val);
        };

      registry.view<Battle, RandomEventCount>().each(collectEntityEventCounts);

      for (const auto& [eventCount, entities] : entitiesByEventCount) {
        const auto [chanceEntities, battleEntities] = entities;
        if (eventCount == 1U) {
          assignClonesToEvents(registry, {}, chanceEntities);
          continue;
        }

        registry.insert<pokesim::tags::CloneFrom>(battleEntities.begin(), battleEntities.end());
        auto clonedEntityMap = clone(registry, eventCount - 1U);

        assignClonesToEvents(registry, clonedEntityMap, chanceEntities);
      }
    }
    else {
      auto assignCloneTags = [&registry](const Battle& battle, auto&&...) {
        registry.emplace<pokesim::tags::CloneFrom>(battle.val);
      };
      registry.view<Battle, Random>().each(assignCloneTags);

      types::entityVector chanceEntities{chanceEntityView.begin(), chanceEntityView.end()};
      auto clonedEntityMap = clone(registry, cloneCount);

      assignClonesToEvents(registry, clonedEntityMap, chanceEntities);
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
  if (simulation.isBattleFormat(BattleFormat::DOUBLES) && !registry.view<RandomStack>().empty()) {
    randomChanceEvent<Random, RandomStack, AssignRandomEvents, UpdateProbabilities, AssignRandomEventsTags...>(
      simulation,
      cloneCount,
      applyChoices,
      assignClonesToEvents,
      updateProbabilities,
      assignArgs...);
  }
}
}  // namespace

PercentChanceLimitResult checkPercentChanceLimits(
  types::probability eventProbability, types::probability probability, const simulate_turn::Options& options) {
  static constexpr auto ProbabilityMax = MechanicConstants::Probability::MAX;
  static constexpr auto ProbabilityMin = MechanicConstants::Probability::MIN;
  static constexpr types::probability PassFailBoundary = (ProbabilityMax - ProbabilityMin) / 2U;

  bool skipBranch = false;
  if (options.hasBranchProbabilityLowerLimit()) {
    types::probability lowestProbability = eventProbability > PassFailBoundary
                                             ? ProbabilityMax - std::min(ProbabilityMax, eventProbability)
                                             : eventProbability;

    skipBranch = lowestProbability * probability <= options.getBranchProbabilityLowerLimit();
  }

  if (
    eventProbability >= options.getRandomChanceUpperLimit() * MechanicConstants::PercentChanceToProbability ||
    (skipBranch && eventProbability >= PassFailBoundary)) {
    return PercentChanceLimitResult::REACHED_PASS_LIMIT;
  }

  if (
    eventProbability <= options.getRandomChanceLowerLimit() * MechanicConstants::PercentChanceToProbability ||
    skipBranch) {
    return PercentChanceLimitResult::REACHED_FAIL_LIMIT;
  }

  return PercentChanceLimitResult::NO_LIMIT_REACHED;
}

template <types::eventPossibilities POSSIBLE_EVENT_COUNT>
void setRandomEventChances(
  types::handle handle, const Simulation& simulation,
  const std::array<types::percentChance, POSSIBLE_EVENT_COUNT>& chances) {
#ifdef POKESIM_DEBUG_CHECK_UTILITIES
  if (chances.back() == MechanicConstants::PercentChance::MAX) {
    for (types::eventPossibilities i = 1U; i < POSSIBLE_EVENT_COUNT; i++) {
      POKESIM_REQUIRE(
        chances[i - 1] <= chances[i],
        "Chances should be a cumulative sum where each value is greater than the last.");
    }
  }
  else {
    types::percentChance chanceSum = 0U;
    for (types::percentChance chance : chances) {
      chanceSum += chance;
      chance = chanceSum;
    }

    POKESIM_REQUIRE(
      chanceSum == MechanicConstants::PercentChance::MAX,
      "The total probability of all possible outcomes should add up to " +
        std::to_string(MechanicConstants::PercentChance::MAX) + "%.");
  }
#endif

  if (useChanceStack(simulation)) {
    handle.registry()
      ->get_or_emplace<RandomEventChancesStack<POSSIBLE_EVENT_COUNT>>(handle.get<Battle>().val)
      .val.emplace_back(chances, handle.entity());
  }
  else {
    handle.emplace<RandomEventChances<POSSIBLE_EVENT_COUNT>>(chances);
  }
}

void setRandomBinaryChanceFromProbability(
  types::handle handle, Battle battle, const Simulation& simulation, types::probability eventProbability) {
  types::probability probability = handle.registry()->get<Probability>(battle.val).val;

  PercentChanceLimitResult limitReached =
    checkPercentChanceLimits(eventProbability, probability, simulation.simulateTurnOptions);

  switch (limitReached) {
    case PercentChanceLimitResult::REACHED_PASS_LIMIT: {
      handle.emplace<tags::RandomEventCheckPassed>();
      return;
    }
    case PercentChanceLimitResult::REACHED_FAIL_LIMIT: {
      handle.emplace<tags::RandomEventCheckFailed>();
      return;
    }
    default: {
      if (useChanceStack(simulation)) {
        handle.registry()
          ->get_or_emplace<RandomBinaryProbabilityStack>(battle.val)
          .val.emplace_back(eventProbability, handle.entity());
      }
      else {
        handle.emplace<RandomBinaryProbability>(eventProbability);
      }
      return;
    };
  }
}

void setRandomBinaryChanceFromPercentChance(
  types::handle handle, Battle battle, const Simulation& simulation, types::percentChance percentChance) {
  setRandomBinaryChanceFromProbability(
    handle,
    battle,
    simulation,
    MechanicConstants::PercentChanceToProbability * percentChance);
}

void setRandomEqualChance(types::handle handle, const Simulation& simulation) {
  if (useChanceStack(simulation)) {
    handle.registry()
      ->get_or_emplace<RandomEqualChanceStack>(handle.get<Battle>().val)
      .val.emplace_back(handle.entity());
  }
  else {
    handle.emplace<tags::RandomEqualChance>();
  }
}

void setRandomEventCounts(
  types::handle handle, Battle battle, const Simulation& simulation,
  types::eventPossibilities (*getPossibleEventCount)(types::handle), bool forRequiredDamageRolls) {
  types::eventPossibilities eventPossibilities = getPossibleEventCount(handle);

  PercentChanceLimitResult limitReached = PercentChanceLimitResult::NO_LIMIT_REACHED;
  if (!forRequiredDamageRolls) {
    types::probability eventProbability = MechanicConstants::Probability::MAX / eventPossibilities;
    types::probability probability = handle.registry()->get<Probability>(battle.val).val;
    limitReached = checkPercentChanceLimits(eventProbability, probability, simulation.simulateTurnOptions);
  }

  if (limitReached == PercentChanceLimitResult::NO_LIMIT_REACHED) {
    if (useChanceStack(simulation)) {
      handle.registry()
        ->get_or_emplace<RandomEventCountStack>(battle.val)
        .val.emplace_back(eventPossibilities, handle.entity());
    }
    else {
      handle.emplace<RandomEventCount>(eventPossibilities);
    }
  }
  else {
    handle.emplace<RandomEventIndex>((types::eventPossibilities)(eventPossibilities / 2U));
  }
}

template <types::eventPossibilities POSSIBLE_EVENT_COUNT>
void randomEventChances(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities) {
  static_assert(
    POSSIBLE_EVENT_COUNT >= 2U,
    "RandomEventChances should only be used for events with more than two options.");
  static_assert(
    POSSIBLE_EVENT_COUNT <= MAX_TYPICAL_RANDOM_OPTIONS,
    "Random events with more options than this should use RandomEqualChance or RandomEventCount");

  auto assignClonesToEvents =
    [](types::registry& registry, const types::ClonedEntityMap& clonedEntityMap, const types::entityVector& originals) {
      for (types::entity original : originals) {
        const auto& cloned = clonedEntityMap.at(original);
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
      }
    };

  auto defaultUpdateProbabilities = [](Simulation& sim) {
    viewUpdateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, tags::RandomEventA>(sim);
    viewUpdateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, tags::RandomEventB>(sim);

    if constexpr (POSSIBLE_EVENT_COUNT >= 3U) {
      viewUpdateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, tags::RandomEventC>(sim);
    }
    if constexpr (POSSIBLE_EVENT_COUNT >= 4U) {
      viewUpdateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, tags::RandomEventD>(sim);
    }
    if constexpr (POSSIBLE_EVENT_COUNT == 5U) {
      viewUpdateProbabilityFromRandomEventChance<POSSIBLE_EVENT_COUNT, tags::RandomEventE>(sim);
    }
  };

  return randomChanceEvent<
    RandomEventChances<POSSIBLE_EVENT_COUNT>,
    RandomEventChancesStack<POSSIBLE_EVENT_COUNT>,
    assignRandomEvent<POSSIBLE_EVENT_COUNT>,
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
    sim.view<updateProbabilityFromRandomEqualChance>(possibleEventCount);
  };

  types::entityIndex cloneCount =
    possibleEventCount > MechanicConstants::DamageRollCount::MAX ? 0U : possibleEventCount - 1U;

  if (updateProbabilities.has_value()) {
    randomChanceEvent<
      tags::RandomEqualChance,
      RandomEqualChanceStack,
      assignRandomEqualChance,
      types::callback,
      tags::RandomEqualChance>(
      simulation,
      cloneCount,
      applyChoices,
      assignIndexToClones,
      updateProbabilities.value(),
      possibleEventCount);
    return;
  }

  randomChanceEvent<
    tags::RandomEqualChance,
    RandomEqualChanceStack,
    assignRandomEqualChance,
    decltype(defaultUpdateProbabilities),
    tags::RandomEqualChance>(
    simulation,
    cloneCount,
    applyChoices,
    assignIndexToClones,
    defaultUpdateProbabilities,
    possibleEventCount);
}

void randomEventCount(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities) {
  auto defaultUpdateProbabilities = [](Simulation& sim) { sim.view<updateProbabilityFromRandomEventCount>(); };

  randomChanceEvent<RandomEventCount, RandomEventCountStack, assignRandomEventCount, types::callback>(
    simulation,
    0U,
    applyChoices,
    assignIndexToClones,
    updateProbabilities.value_or(defaultUpdateProbabilities));
}

void randomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities) {
  auto assignClonesToEvents =
    [](types::registry& registry, const types::ClonedEntityMap& clonedEntityMap, const types::entityVector& originals) {
      for (types::entity original : originals) {
        const auto& cloned = clonedEntityMap.at(original);
        registry.emplace<tags::RandomEventCheckPassed>(original);
        registry.emplace<tags::RandomEventCheckFailed>(cloned[0]);
      }
    };

  auto defaultUpdateProbabilities = [](Simulation& sim) {
    sim.view<
      updateProbabilityFromRandomBinaryChance<tags::RandomEventCheckPassed>,
      Tags<tags::RandomEventCheckPassed>>();
    sim.view<
      updateProbabilityFromRandomBinaryChance<tags::RandomEventCheckFailed>,
      Tags<tags::RandomEventCheckFailed>>();
  };

  randomChanceEvent<RandomBinaryProbability, RandomBinaryProbabilityStack, assignRandomBinaryEvent, types::callback>(
    simulation,
    1U,
    applyChoices,
    assignClonesToEvents,
    updateProbabilities.value_or(defaultUpdateProbabilities));
}

template void randomEventChances<2U>(Simulation&, types::callback, types::optionalCallback);
template void randomEventChances<3U>(Simulation&, types::callback, types::optionalCallback);
template void randomEventChances<4U>(Simulation&, types::callback, types::optionalCallback);
template void randomEventChances<5U>(Simulation&, types::callback, types::optionalCallback);

template void setRandomEventChances<2U>(types::handle, const Simulation&, const std::array<types::percentChance, 2U>&);
template void setRandomEventChances<3U>(types::handle, const Simulation&, const std::array<types::percentChance, 3U>&);
template void setRandomEventChances<4U>(types::handle, const Simulation&, const std::array<types::percentChance, 4U>&);
template void setRandomEventChances<5U>(types::handle, const Simulation&, const std::array<types::percentChance, 5U>&);
}  // namespace pokesim::internal
