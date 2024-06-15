#pragma once

#include <Types/Entity.hpp>
#include <Types/Random.hpp>
#include <Types/State.hpp>

namespace pokesim {
class Simulation;

void setRandomBinaryChoice(
  const Simulation& simulation, types::handle battleHandle, types::percentChance percentChance);
template <std::uint8_t POSSIBLE_EVENT_COUNT>
void randomChance(Simulation& simulation);
void randomBinaryChance(Simulation& simulation);
void clearRandomChanceResult(Simulation& simulation);

void sampleRandomChance(Simulation& simulation);
}  // namespace pokesim