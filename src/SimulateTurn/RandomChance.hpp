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
template <typename Type>
void updateProbability(Probability& currentProbability, Type percentChance);
template <std::uint8_t POSSIBLE_EVENT_COUNT, typename RandomEventTag>
void updateProbabilityFromRandomChance(
  types::registry& registy, const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances, const Battle& battle);
template <bool Reciprocal, typename RandomEventTag>
void updateProbabilityFromRandomBinaryChance(
  types::registry& registy, const RandomBinaryEventChance& eventChance, const Battle& battle);

template <BattleFormat Format>
void setBinaryChanceByFormat(types::handle handle, types::percentChance percentChance);
template <typename Type>
bool checkPercentChanceLimits(
  types::handle handle, Type percentChance, types::percentChance autoPassLimit, types::percentChance autoFailLimit);

template <typename Component, BattleFormat Format>
void setRandomBinaryChoice(
  types::handle handle, const Component& percentChance, types::percentChance autoPassLimit,
  types::percentChance autoFailLimit);
template <typename Component, BattleFormat Format>
void setReciprocalRandomBinaryChoice(
  types::handle handle, const Component& percentChance, types::percentChance autoPassLimit,
  types::percentChance autoFailLimit);

template <bool Reciprocal>
void randomBinaryChance(Simulation& simulation, void (*handleRandomEventChoice)(Simulation&));

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void assignRandomEvent(
  types::handle handle, const Battle& battle, const RandomEventChances<POSSIBLE_EVENT_COUNT>& eventChances);
void assignRandomBinaryEvent(types::handle handle, const Battle& battle, const RandomBinaryEventChance& eventChance);
void assignReciprocalRandomBinaryEvent(
  types::handle handle, const Battle& battle, const RandomBinaryEventChance& eventChance);

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void placeRandomEventChanceFromStack(types::handle handle, RandomEventChancesStack<POSSIBLE_EVENT_COUNT>& stack);
void placeRandomBinaryEventChanceFromStack(types::handle handle, RandomBinaryEventChanceStack& stack);
}  // namespace internal

template <std::uint8_t POSSIBLE_EVENT_COUNT, BattleFormat Format, bool CumulativeSumChances>
void setRandomChoice(types::handle handle, std::array<types::percentChance, POSSIBLE_EVENT_COUNT> chances);
template <typename Component, typename... T>
void setRandomBinaryChoice(Simulation& simulation);
template <typename Component, typename... T>
void setReciprocalRandomBinaryChoice(Simulation& simulation);

template <std::uint8_t POSSIBLE_EVENT_COUNT>
void randomChance(Simulation& simulation, void (*handleRandomEventChoice)(Simulation&));
void randomBinaryChance(Simulation& simulation, void (*handleRandomEventChoice)(Simulation&));
void reciprocalRandomBinaryChance(Simulation& simulation, void (*handleRandomEventChoice)(Simulation&));
void clearRandomChanceResult(Simulation& simulation);

void sampleRandomChance(Simulation& simulation);
}  // namespace pokesim