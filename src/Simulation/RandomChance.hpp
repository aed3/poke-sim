#pragma once

#include <Components/RandomEventCheck.hpp>
#include <Types/Entity.hpp>
#include <Types/Random.hpp>
#include <Types/State.hpp>

namespace pokesim {
class Simulation;
struct RngSeed;
struct Probability;

namespace internal {
void updateProbability(Probability& currentProbability, types::percentChance percentChance);

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void assignRandomEvent(
  types::handle battleHandle, const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventCheck, RngSeed& rngSeed,
  Probability& probability);
void assignRandomBinaryEvent(
  types::handle battleHandle, const RandomBinaryEventChance& eventCheck, RngSeed& rngSeed, Probability& probability);
}  // namespace internal

void setRandomBinaryChoice(
  const Simulation& simulation, types::handle battleHandle, types::percentChance percentChance);
template <std::uint8_t POSSIBLE_EVENT_COUNT>
void randomChance(Simulation& simulation);
void randomBinaryChance(Simulation& simulation);
void clearRandomChanceResult(Simulation& simulation);

void sampleRandomChance(Simulation& simulation);
}  // namespace pokesim