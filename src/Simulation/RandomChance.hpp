#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/BattleFormat.hpp>
#include <Types/Random.hpp>
#include <Types/State.hpp>

namespace pokesim {
class Simulation;
struct RngSeed;
struct Battle;
struct Probability;
struct RandomBinaryEventChance;
struct RandomBinaryEventChanceStack;
template <std::uint8_t RANDOM_OPTIONS>
struct RandomEventChances;
template <std::uint8_t RANDOM_OPTIONS>
struct RandomEventChancesStack;

namespace internal {
void updateProbability(Probability& currentProbability, types::percentChance percentChance);

template <typename Component, BattleFormat Format>
void setRandomBinaryChoice(
  types::registry& registry, const Battle& battle, const Component& percentChance, types::percentChance autoPassLimit,
  types::percentChance autoFailLimit);

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void assignRandomEvent(
  types::handle battleHandle, const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventCheck, RngSeed& rngSeed,
  Probability& probability);
void assignRandomBinaryEvent(
  types::handle battleHandle, const RandomBinaryEventChance& eventCheck, RngSeed& rngSeed, Probability& probability);

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void placeRandomEventChanceFromStack(types::handle battleHandle, RandomEventChancesStack<POSSIBLE_EVENT_COUNT>& stack);
void placeRandomBinaryEventChanceFromStack(types::handle battleHandle, RandomBinaryEventChanceStack& stack);
}  // namespace internal

template <std::uint8_t POSSIBLE_EVENT_COUNT, typename Component>
void setRandomChoice(Simulation& simulation);
template <typename Component, typename... Tags>
void setRandomBinaryChoice(Simulation& simulation);

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void randomChance(Simulation& simulation);
void randomBinaryChance(Simulation& simulation);
void clearRandomChanceResult(Simulation& simulation);

void sampleRandomChance(Simulation& simulation);
}  // namespace pokesim