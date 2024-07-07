#pragma once

#include <Components/EntityHolders/Battle.hpp>
#include <Components/RandomEventInputs.hpp>
#include <Components/RandomEventOutputs.hpp>
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

template <typename Type>
bool checkPercentChanceLimits(
  types::handle handle, Type percentChance, types::percentChance autoPassLimit, types::percentChance autoFailLimit) {
  if (percentChance >= autoPassLimit) {
    handle.emplace<tags::RandomEventCheckPassed>();
    return true;
  }

  if (percentChance <= autoFailLimit) {
    handle.emplace<tags::RandomEventCheckFailed>();
    return true;
  }
  return false;
}

template <typename Component, BattleFormat Format>
void setRandomBinaryChoice(
  types::handle handle, const Component& percentChance, types::percentChance autoPassLimit,
  types::percentChance autoFailLimit) {
  if (!checkPercentChanceLimits(handle, percentChance.val, autoPassLimit, autoFailLimit)) {
    setBinaryChanceByFormat<Format>(handle, percentChance.val);
  }
}

template <typename Component, BattleFormat Format>
void setReciprocalRandomBinaryChoice(
  types::handle handle, const Component& percentChance, types::percentChance autoPassLimit,
  types::percentChance autoFailLimit) {
  types::probability passChance = 100.0F / (types::probability)percentChance.val;
  if (!checkPercentChanceLimits(handle, passChance, autoPassLimit, autoFailLimit)) {
    setBinaryChanceByFormat<Format>(handle, percentChance.val);
  }
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

template <BattleFormat Format>
void setRandomEventCounts(types::handle handle, types::eventPossibilities possibleEventCount) {
  if constexpr (Format == BattleFormat::SINGLES_BATTLE_FORMAT) {
    handle.emplace<RandomEventCount>(possibleEventCount);
  }
  else {
    handle.registry()
      ->get_or_emplace<RandomEventCountStack>(handle.get<Battle>().val)
      .val.emplace_back(possibleEventCount, handle.entity());
  }
}
}  // namespace internal

template <types::eventPossibilities POSSIBLE_EVENT_COUNT, typename... T>
void setRandomChoice(
  Simulation& simulation, std::array<types::percentChance, POSSIBLE_EVENT_COUNT> chances,
  const bool cumulativeSumChances) {
  if (cumulativeSumChances) {
    types::percentChance chanceSum = 0;
    for (types::percentChance& chance : chances) {
      chanceSum += chance;
      chance = chanceSum;
    }

    ENTT_ASSERT(chanceSum == 100, "The total probability of all possible outcomes should add up to 100%.");
  }
  else {
    ENTT_ASSERT(chances.back() == 100, "The total probability of all possible outcomes should add up to 100%.");
    for (types::eventPossibilities i = 1; i < POSSIBLE_EVENT_COUNT; i++) {
      ENTT_ASSERT(
        chances[i - 1] <= chances[i],
        "Chances should be a cumulative sum where each value is greater than the last.");
    }
  }

  if (simulation.battleFormat == BattleFormat::SINGLES_BATTLE_FORMAT) {
    simulation.view<internal::setRandomChoice<POSSIBLE_EVENT_COUNT, BattleFormat::SINGLES_BATTLE_FORMAT>, Tags<T...>>(
      chances);
  }
  else {
    simulation.view<internal::setRandomChoice<POSSIBLE_EVENT_COUNT, BattleFormat::DOUBLES_BATTLE_FORMAT>, Tags<T...>>(
      chances);
  }
}

template <typename Component, typename... T>
void setRandomBinaryChoice(Simulation& simulation) {
  types::percentChance autoPassLimit = simulation.simulateTurnOptions.randomChanceUpperLimit.value_or(100);
  types::percentChance autoFailLimit = simulation.simulateTurnOptions.randomChanceLowerLimit.value_or(0);
  if (simulation.battleFormat == BattleFormat::SINGLES_BATTLE_FORMAT) {
    simulation.view<internal::setRandomBinaryChoice<Component, BattleFormat::SINGLES_BATTLE_FORMAT>, Tags<T...>>(
      autoPassLimit,
      autoFailLimit);
  }
  else {
    simulation.view<internal::setRandomBinaryChoice<Component, BattleFormat::DOUBLES_BATTLE_FORMAT>, Tags<T...>>(
      autoPassLimit,
      autoFailLimit);
  }
}

template <typename Component, typename... T>
void setReciprocalRandomBinaryChoice(Simulation& simulation) {
  types::percentChance autoPassLimit = simulation.simulateTurnOptions.randomChanceUpperLimit.value_or(100);
  types::percentChance autoFailLimit = simulation.simulateTurnOptions.randomChanceLowerLimit.value_or(0);
  if (simulation.battleFormat == BattleFormat::SINGLES_BATTLE_FORMAT) {
    simulation
      .view<internal::setReciprocalRandomBinaryChoice<Component, BattleFormat::SINGLES_BATTLE_FORMAT>, Tags<T...>>(
        autoPassLimit,
        autoFailLimit);
  }
  else {
    simulation
      .view<internal::setReciprocalRandomBinaryChoice<Component, BattleFormat::DOUBLES_BATTLE_FORMAT>, Tags<T...>>(
        autoPassLimit,
        autoFailLimit);
  }
}

template <typename... T>
void setRandomEqualChoice(Simulation& simulation) {
  if (simulation.battleFormat == BattleFormat::SINGLES_BATTLE_FORMAT) {
    simulation.view<internal::setRandomEqualChoice<BattleFormat::SINGLES_BATTLE_FORMAT>, Tags<T...>>();
  }
  else {
    simulation.view<internal::setRandomEqualChoice<BattleFormat::DOUBLES_BATTLE_FORMAT>, Tags<T...>>();
  }
}

template <typename... T>
void setRandomEventCounts(Simulation& simulation, types::eventPossibilities possibleEventCount) {
  if (simulation.battleFormat == BattleFormat::SINGLES_BATTLE_FORMAT) {
    simulation.view<internal::setRandomEventCounts<BattleFormat::SINGLES_BATTLE_FORMAT>, Tags<T...>>(
      possibleEventCount);
  }
  else {
    simulation.view<internal::setRandomEventCounts<BattleFormat::DOUBLES_BATTLE_FORMAT>, Tags<T...>>(
      possibleEventCount);
  }
}

template <types::eventPossibilities POSSIBLE_EVENT_COUNT>
void randomEventChances(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities = std::nullopt);
void randomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities = std::nullopt);
void reciprocalRandomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities = std::nullopt);
void randomEqualChance(
  Simulation& simulation, types::eventPossibilities possibleEventCount, types::callback applyChoices,
  types::optionalCallback updateProbabilities = std::nullopt);
void randomEventCount(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities = std::nullopt);

void clearRandomChanceResult(Simulation& simulation);
}  // namespace pokesim