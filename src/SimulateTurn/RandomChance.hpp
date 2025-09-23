#pragma once

#include <Components/EntityHolders/Battle.hpp>
#include <Components/Probability.hpp>
#include <Components/RandomEventInputs.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Config/Require.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/BattleFormat.hpp>
#include <Types/Random.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <optional>

namespace pokesim {
namespace internal {
template <types::eventPossibilities POSSIBLE_EVENT_COUNT, BattleFormat Format>
void setRandomChoice(types::handle handle, const std::array<types::percentChance, POSSIBLE_EVENT_COUNT>& chances) {
  if constexpr (Format == BattleFormat::SINGLES_BATTLE_FORMAT) {
    handle.emplace<RandomEventChances<POSSIBLE_EVENT_COUNT>>(chances);
  }
  else {
    handle.registry()
      ->get_or_emplace<RandomEventChancesStack<POSSIBLE_EVENT_COUNT>>(handle.get<Battle>().val)
      .val.emplace_back(chances, handle.entity());
  }
}

template <BattleFormat Format>
void setBinaryChanceByFormat(types::handle handle, types::percentChance percentChance) {
  if constexpr (Format == BattleFormat::SINGLES_BATTLE_FORMAT) {
    handle.emplace<RandomBinaryChance>(percentChance);
  }
  else {
    handle.registry()
      ->get_or_emplace<RandomBinaryChanceStack>(handle.get<Battle>().val)
      .val.emplace_back(percentChance, handle.entity());
  }
}

enum class PercentChanceLimitResult : std::uint8_t {
  NO_LIMIT_REACHED = 0U,
  REACHED_PASS_LIMIT,
  REACHED_FAIL_LIMIT,
};

inline PercentChanceLimitResult checkPercentChanceLimits(
  types::percentChance percentChance, types::probability probability, const simulate_turn::Options& options) {
  const auto& autoPassLimit = options.randomChanceUpperLimit;
  const auto& autoFailLimit = options.randomChanceLowerLimit;
  const auto& branchProbabilityLowerLimit = options.branchProbabilityLowerLimit;

  bool skipBranch = false;
  if (branchProbabilityLowerLimit.has_value()) {
    skipBranch =
      percentChance * probability <= branchProbabilityLowerLimit.value() * MechanicConstants::PercentChance::MAX;
  }
  const types::percentChance PASS_FAIL_BOUNDARY =
    (MechanicConstants::PercentChance::MAX - MechanicConstants::PercentChance::MIN) / 2U;

  if (
    percentChance >= autoPassLimit.value_or(MechanicConstants::PercentChance::MAX) ||
    (skipBranch && percentChance >= PASS_FAIL_BOUNDARY)) {
    return PercentChanceLimitResult::REACHED_PASS_LIMIT;
  }

  if (percentChance <= autoFailLimit.value_or(MechanicConstants::PercentChance::MIN) || skipBranch) {
    return PercentChanceLimitResult::REACHED_FAIL_LIMIT;
  }

  return PercentChanceLimitResult::NO_LIMIT_REACHED;
}

template <BattleFormat Format>
void setBinaryChanceFromChanceLimit(
  types::handle handle, types::percentChance passChance, types::percentChance percentChance,
  types::probability probability, const simulate_turn::Options& options) {
  PercentChanceLimitResult limitReached = checkPercentChanceLimits(passChance, probability, options);

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
      setBinaryChanceByFormat<Format>(handle, percentChance);
      return;
    };
  }
}

template <typename PercentChanceComponent, BattleFormat Format>
void setRandomBinaryChoice(
  types::handle handle, const PercentChanceComponent& percentChance, const Battle& battle,
  const simulate_turn::Options& options) {
  types::probability probability = handle.registry()->get<Probability>(battle.val).val;

  setBinaryChanceFromChanceLimit<Format>(handle, percentChance.val, percentChance.val, probability, options);
}

template <typename PercentChanceComponent, BattleFormat Format>
void setReciprocalRandomBinaryChoice(
  types::handle handle, const PercentChanceComponent& percentChance, const Battle& battle,
  const simulate_turn::Options& options) {
  types::percentChance passChance = MechanicConstants::PercentChance::MAX / percentChance.val;
  types::probability probability = handle.registry()->get<Probability>(battle.val).val;

  setBinaryChanceFromChanceLimit<Format>(handle, passChance, percentChance.val, probability, options);
}

template <BattleFormat Format>
void setRandomEqualChoice(types::handle handle) {
  if constexpr (Format == BattleFormat::SINGLES_BATTLE_FORMAT) {
    handle.emplace<tags::RandomEqualChance>();
  }
  else {
    handle.registry()
      ->get_or_emplace<RandomEqualChanceStack>(handle.get<Battle>().val)
      .val.emplace_back(handle.entity());
  }
}

template <BattleFormat Format, auto GetPossibleEventCount>
void setRandomEventCounts(
  types::handle handle, const Battle& battle, const simulate_turn::Options& options, bool forRequiredDamageRolls) {
  auto eventCount = GetPossibleEventCount(handle);

  PercentChanceLimitResult limitReached = PercentChanceLimitResult::NO_LIMIT_REACHED;
  if (!forRequiredDamageRolls) {
    types::percentChance passChance =
      types::percentChance((types::probability)MechanicConstants::PercentChance::MAX / eventCount);
    types::probability probability = handle.registry()->get<Probability>(battle.val).val;
    limitReached = checkPercentChanceLimits(passChance, probability, options);
  }

  if (limitReached == PercentChanceLimitResult::NO_LIMIT_REACHED) {
    if constexpr (Format == BattleFormat::SINGLES_BATTLE_FORMAT) {
      handle.emplace<RandomEventCount>(eventCount);
    }
    else {
      handle.registry()
        ->get_or_emplace<RandomEventCountStack>(handle.get<Battle>().val)
        .val.emplace_back(eventCount, handle.entity());
    }
  }
  else {
    handle.emplace<RandomEventIndex>((types::eventPossibilities)(eventCount / 2U));
  }
}

template <types::eventPossibilities POSSIBLE_EVENT_COUNT, typename... SelectionTags>
void setRandomChoice(
  Simulation& simulation, std::array<types::percentChance, POSSIBLE_EVENT_COUNT> chances,
  const bool cumulativeSumChances) {
  if (cumulativeSumChances) {
    types::percentChance chanceSum = 0U;
    for (types::percentChance& chance : chances) {
      chanceSum += chance;
      chance = chanceSum;
    }

    POKESIM_REQUIRE(
      chanceSum == MechanicConstants::PercentChance::MAX,
      "The total probability of all possible outcomes should add up to " +
        std::to_string(MechanicConstants::PercentChance::MAX) + "%.");
  }
  else {
    POKESIM_REQUIRE(
      chances.back() == MechanicConstants::PercentChance::MAX,
      "The total probability of all possible outcomes should add up to " +
        std::to_string(MechanicConstants::PercentChance::MAX) + "%.");
    for (types::eventPossibilities i = 1U; i < POSSIBLE_EVENT_COUNT; i++) {
      POKESIM_REQUIRE(
        chances[i - 1] <= chances[i],
        "Chances should be a cumulative sum where each value is greater than the last.");
    }
  }

  if (simulation.battleFormat() == BattleFormat::SINGLES_BATTLE_FORMAT) {
    simulation.view<setRandomChoice<POSSIBLE_EVENT_COUNT, BattleFormat::SINGLES_BATTLE_FORMAT>, Tags<SelectionTags...>>(
      chances);
  }
  else {
    simulation.view<setRandomChoice<POSSIBLE_EVENT_COUNT, BattleFormat::DOUBLES_BATTLE_FORMAT>, Tags<SelectionTags...>>(
      chances);
  }
}

template <typename PercentChanceComponent, typename... SelectionTags>
void setRandomBinaryChoice(Simulation& simulation) {
  const auto& options = simulation.simulateTurnOptions;

  if (simulation.battleFormat() == BattleFormat::SINGLES_BATTLE_FORMAT) {
    simulation
      .view<setRandomBinaryChoice<PercentChanceComponent, BattleFormat::SINGLES_BATTLE_FORMAT>, Tags<SelectionTags...>>(
        options);
  }
  else {
    simulation
      .view<setRandomBinaryChoice<PercentChanceComponent, BattleFormat::DOUBLES_BATTLE_FORMAT>, Tags<SelectionTags...>>(
        options);
  }
}

template <typename PercentChanceComponent, typename... SelectionTags>
void setReciprocalRandomBinaryChoice(Simulation& simulation) {
  const auto& options = simulation.simulateTurnOptions;

  if (simulation.battleFormat() == BattleFormat::SINGLES_BATTLE_FORMAT) {
    simulation.view<
      setReciprocalRandomBinaryChoice<PercentChanceComponent, BattleFormat::SINGLES_BATTLE_FORMAT>,
      Tags<SelectionTags...>>(options);
  }
  else {
    simulation.view<
      setReciprocalRandomBinaryChoice<PercentChanceComponent, BattleFormat::DOUBLES_BATTLE_FORMAT>,
      Tags<SelectionTags...>>(options);
  }
}

template <typename... SelectionTags>
void setRandomEqualChoice(Simulation& simulation) {
  if (simulation.battleFormat() == BattleFormat::SINGLES_BATTLE_FORMAT) {
    simulation.view<setRandomEqualChoice<BattleFormat::SINGLES_BATTLE_FORMAT>, Tags<SelectionTags...>>();
  }
  else {
    simulation.view<setRandomEqualChoice<BattleFormat::DOUBLES_BATTLE_FORMAT>, Tags<SelectionTags...>>();
  }
}

template <auto GetPossibleEventCount, typename... SelectionTags>
void setRandomEventCounts(Simulation& simulation, bool forRequiredDamageRolls) {
  const auto& options = simulation.simulateTurnOptions;

  if (simulation.battleFormat() == BattleFormat::SINGLES_BATTLE_FORMAT) {
    simulation
      .view<setRandomEventCounts<BattleFormat::SINGLES_BATTLE_FORMAT, GetPossibleEventCount>, Tags<SelectionTags...>>(
        options,
        forRequiredDamageRolls);
  }
  else {
    simulation
      .view<setRandomEventCounts<BattleFormat::DOUBLES_BATTLE_FORMAT, GetPossibleEventCount>, Tags<SelectionTags...>>(
        options,
        forRequiredDamageRolls);
  }
}

template <types::eventPossibilities POSSIBLE_EVENT_COUNT>
void randomEventChances(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities);
void randomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities);
void reciprocalRandomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities);
void randomEqualChance(
  Simulation& simulation, types::eventPossibilities possibleEventCount, types::callback applyChoices,
  types::optionalCallback updateProbabilities);
void randomEventCount(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities);

void clearRandomChanceResult(Simulation& simulation);
}  // namespace internal

template <types::eventPossibilities POSSIBLE_EVENT_COUNT, typename... SelectionTags>
void runRandomEventChances(
  Simulation& simulation, std::array<types::percentChance, POSSIBLE_EVENT_COUNT> chances,
  const bool cumulativeSumChances, types::callback applyChoices,
  types::optionalCallback updateProbabilities = std::nullopt) {
  internal::setRandomChoice<POSSIBLE_EVENT_COUNT, SelectionTags...>(simulation, chances, cumulativeSumChances);

  if (!simulation.registry.view<RandomEventChances<POSSIBLE_EVENT_COUNT>>().empty()) {
    internal::randomEventChances<POSSIBLE_EVENT_COUNT>(simulation, applyChoices, updateProbabilities);
  }
}

template <typename PercentChanceComponent, typename... SelectionTags>
void runRandomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities = std::nullopt) {
  internal::setRandomBinaryChoice<PercentChanceComponent, SelectionTags...>(simulation);
  internal::randomBinaryChance(simulation, applyChoices, updateProbabilities);
}

template <typename PercentChanceComponent, typename... SelectionTags>
void runReciprocalRandomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities = std::nullopt) {
  internal::setReciprocalRandomBinaryChoice<PercentChanceComponent, SelectionTags...>(simulation);
  internal::reciprocalRandomBinaryChance(simulation, applyChoices, updateProbabilities);
}

template <typename... SelectionTags>
void runRandomEqualChance(
  Simulation& simulation, types::eventPossibilities possibleEventCount, types::callback applyChoices,
  types::optionalCallback updateProbabilities = std::nullopt) {
  internal::setRandomEqualChoice<SelectionTags...>(simulation);
  internal::randomEqualChance(simulation, possibleEventCount, applyChoices, updateProbabilities);
}

template <auto GetPossibleEventCount, typename... SelectionTags>
void runRandomEventCount(
  Simulation& simulation, bool forRequiredDamageRolls, types::callback applyChoices,
  types::optionalCallback updateProbabilities = std::nullopt) {
  internal::setRandomEventCounts<GetPossibleEventCount, SelectionTags...>(simulation, forRequiredDamageRolls);
  internal::randomEventCount(simulation, applyChoices, updateProbabilities);
}
}  // namespace pokesim