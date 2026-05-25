#pragma once

#include <Components/EntityHolders/Battle.hpp>
#include <Components/Probability.hpp>
#include <Components/RandomEventInputs.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Config/Require.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Random.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>

namespace pokesim {
namespace internal {
template <types::eventPossibilities POSSIBLE_EVENT_COUNT>
void setRandomEventChances(
  types::handle handle, const Simulation& simulation,
  const std::array<types::percentChance, POSSIBLE_EVENT_COUNT>& chances);

enum class PercentChanceLimitResult : std::uint8_t {
  NO_LIMIT_REACHED = 0U,
  REACHED_PASS_LIMIT,
  REACHED_FAIL_LIMIT,
};

PercentChanceLimitResult checkPercentChanceLimits(
  types::probability eventProbability, types::probability probability, const simulate_turn::Options& options);

void setRandomBinaryChanceFromProbability(
  types::handle handle, Battle battle, const Simulation& simulation, types::probability eventProbability);

void setRandomBinaryChanceFromPercentChance(
  types::handle handle, Battle battle, const Simulation& simulation, types::percentChance percentChance);

template <typename PercentChanceComponent>
void setRandomBinaryChance(
  types::handle handle, PercentChanceComponent percentChance, Battle battle, const Simulation& simulation) {
  setRandomBinaryChanceFromPercentChance(handle, battle, simulation, percentChance.val);
}

template <typename PercentChanceComponent>
void setReciprocalRandomBinaryChance(
  types::handle handle, PercentChanceComponent percentChance, Battle battle, const Simulation& simulation) {
  setRandomBinaryChanceFromProbability(
    handle,
    battle,
    simulation,
    MechanicConstants::Probability::MAX / percentChance.val);
}

void setRandomEqualChance(types::handle handle, const Simulation& simulation);
void setRandomEventCounts(
  types::handle handle, Battle battle, const Simulation& simulation,
  types::eventPossibilities (*getPossibleEventCount)(types::handle), bool forRequiredDamageRolls);

template <types::eventPossibilities POSSIBLE_EVENT_COUNT>
void randomEventChances(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities);
void randomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities);
void randomEqualChance(
  Simulation& simulation, types::eventPossibilities possibleEventCount, types::callback applyChoices,
  types::optionalCallback updateProbabilities);
void randomEventCount(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities);
}  // namespace internal

template <types::eventPossibilities POSSIBLE_EVENT_COUNT, typename... SelectionTags>
void runRandomEventChances(
  Simulation& simulation, std::array<types::percentChance, POSSIBLE_EVENT_COUNT> chances, types::callback applyChoices,
  types::optionalCallback updateProbabilities = std::nullopt) {
  simulation.view<internal::setRandomEventChances<POSSIBLE_EVENT_COUNT>, Tags<SelectionTags...>>(simulation, chances);

  if (!simulation.registry.view<internal::RandomEventChances<POSSIBLE_EVENT_COUNT>>().empty()) {
    internal::randomEventChances<POSSIBLE_EVENT_COUNT>(simulation, applyChoices, updateProbabilities);
  }
}

template <typename PercentChanceComponent, typename... SelectionTags>
void runRandomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities = std::nullopt) {
  simulation.view<internal::setRandomBinaryChance<PercentChanceComponent>, Tags<SelectionTags...>>(simulation);

  internal::randomBinaryChance(simulation, applyChoices, updateProbabilities);
}

template <typename PercentChanceComponent, typename... SelectionTags>
void runReciprocalRandomBinaryChance(
  Simulation& simulation, types::callback applyChoices, types::optionalCallback updateProbabilities = std::nullopt) {
  simulation.view<internal::setReciprocalRandomBinaryChance<PercentChanceComponent>, Tags<SelectionTags...>>(
    simulation);

  internal::randomBinaryChance(simulation, applyChoices, updateProbabilities);
}

template <typename... SelectionTags>
void runRandomEqualChance(
  Simulation& simulation, types::eventPossibilities possibleEventCount, types::callback applyChoices,
  types::optionalCallback updateProbabilities = std::nullopt) {
  simulation.view<internal::setRandomEqualChance, Tags<SelectionTags...>>();

  internal::randomEqualChance(simulation, possibleEventCount, applyChoices, updateProbabilities);
}

template <typename... SelectionTags>
void runRandomEventCount(
  Simulation& simulation, types::eventPossibilities (*getPossibleEventCount)(types::handle),
  types::callback applyChoices, types::optionalCallback updateProbabilities = std::nullopt,
  bool forRequiredDamageRolls = false) {
  simulation.view<internal::setRandomEventCounts, Tags<SelectionTags...>>(
    simulation,
    getPossibleEventCount,
    forRequiredDamageRolls);

  internal::randomEventCount(simulation, applyChoices, updateProbabilities);
}
}  // namespace pokesim
