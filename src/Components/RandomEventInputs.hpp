#pragma once

#include <Types/Constants.hpp>
#include <Types/Random.hpp>
#include <Types/State.hpp>
#include <array>

namespace pokesim::internal {
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

// Used for random events that have a small number of outcomes that can happen and the chance each event can happen is
// not equal (i.e. how many times a multi-hit move hits, Effect Spore)
template <types::eventPossibilities RANDOM_OPTIONS>
struct RandomEventChances {
  std::array<types::percentChance, RANDOM_OPTIONS> val{};
  static_assert(RANDOM_OPTIONS >= 2U, "RandomEventChances should only be used for events with more than two options.");
  static_assert(
    RANDOM_OPTIONS <= internal::MAX_TYPICAL_RANDOM_OPTIONS,
    "Random events with more options than this should use RandomEqualChance or RandomEventCount");

  types::probability probabilityA() const { return Constants::PercentChanceToProbability * val[0]; }
  types::probability probabilityB() const { return Constants::PercentChanceToProbability * (val[1] - val[0]); }
  types::probability probabilityC() const {
    static_assert(RANDOM_OPTIONS >= 3U, "This function is only for events with more than 2 outcomes.");
    return Constants::PercentChanceToProbability * (val[2] - val[1]);
  }
  types::probability probabilityD() const {
    static_assert(RANDOM_OPTIONS >= 4U, "This function is only for events with more than 3 outcomes.");
    return Constants::PercentChanceToProbability * (val[3] - val[2]);
  }
  types::probability probabilityE() const {
    static_assert(RANDOM_OPTIONS == 5U, "This function is only for events with 5 outcomes.");
    return Constants::PercentChanceToProbability * (val[4] - val[3]);
  }
};

// Used for random events that always have two outcomes where the chance the events happens may not be equal (i.e. move
// accuracy checks, secondary move effects)
struct RandomBinaryProbability {
  types::probability val = Constants::Probability::DEFAULT;

  types::probability pass() const { return val; }
  types::probability fail() const { return Constants::Probability::MAX - pass(); }
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

  types::probability probability() const { return Constants::Probability::MAX / val; }
};

template <types::eventPossibilities RANDOM_OPTIONS>
struct RandomEventChancesStack : internal::RandomEventStack<RandomEventChances<RANDOM_OPTIONS>> {};
struct RandomBinaryProbabilityStack : internal::RandomEventStack<RandomBinaryProbability> {};
struct RandomEventCountStack : internal::RandomEventStack<RandomEventCount> {};
struct RandomEqualChanceStack : internal::RandomEventStack<> {};
}  // namespace pokesim::internal
