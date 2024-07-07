#pragma once

#include <Types/Random.hpp>
#include <Types/State.hpp>
#include <array>

namespace pokesim {
namespace internal {
// Moves such as Metronome and Psywave, the Forewarn ability, and damage rolls have more random options than this, but
// those cases will be handled separately
const types::eventPossibilities MAX_TYPICAL_RANDOM_OPTIONS = 5U;

template <typename T = void>
struct RandomEventStack;

template <>
struct RandomEventStack<void> {
  types::targets<types::entity> val{};
};

template <typename T>
struct RandomEventStack {
  types::targets<std::pair<decltype(T::val), types::entity>> val{};
};
}  // namespace internal

// Used for random events that have a small number of outcomes that can happen and the chance each event can happen is
// not equal (i.e. how many times a multi-hit move hits, Effect Spore)
template <types::eventPossibilities RANDOM_OPTIONS>
struct RandomEventChances {
  std::array<types::percentChance, RANDOM_OPTIONS> val{};
  static_assert(RANDOM_OPTIONS <= internal::MAX_TYPICAL_RANDOM_OPTIONS);

  types::percentChance chanceA() const { return val[0]; }
  types::percentChance chanceB() const { return val[1] - val[0]; }
  types::percentChance chanceC() const {
    static_assert(RANDOM_OPTIONS >= 3U);
    return val[2] - val[1];
  }
  types::percentChance chanceD() const {
    static_assert(RANDOM_OPTIONS >= 4U);
    return val[3] - val[2];
  }
  types::percentChance chanceE() const {
    static_assert(RANDOM_OPTIONS == 5U);
    return val[4] - val[3];
  }
};

// Used for random events that always have two outcomes where the chance the events happens may not be equal (i.e. move
// accuracy checks, secondary move effects)
struct RandomBinaryChance {
  types::percentChance val = 100U;

  types::percentChance pass() const { return val; }
  types::percentChance fail() const { return 100U - pass(); }
  types::probability reciprocalPass() const { return 100.0F / (types::probability)pass(); }
  types::probability reciprocalFail() const { return 100U - reciprocalPass(); }
};

namespace tags {
// Used for when the number of outcomes a random event can have is always the same (i.e. damage rolls, Psywave) and the
// chance each of those events can happen is equal
struct RandomEqualChance {};
}  // namespace tags

// Used for when the number of outcomes a random event can have depends on the situation (i.e. speed ties, Trace) and
// the chance each of those events can happen is equal
struct RandomEventCount {
  types::eventPossibilities val = 0U;
};

template <types::eventPossibilities RANDOM_OPTIONS>
struct RandomEventChancesStack : internal::RandomEventStack<RandomEventChances<RANDOM_OPTIONS>> {};
struct RandomBinaryChanceStack : internal::RandomEventStack<RandomBinaryChance> {};
struct RandomEventCountStack : internal::RandomEventStack<RandomEventCount> {};
struct RandomEqualChanceStack : internal::RandomEventStack<> {};
}  // namespace pokesim