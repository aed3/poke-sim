#pragma once

#include <Components/RNGSeed.hpp>
#include <Config/Require.hpp>
#include <Types/Random.hpp>

// Adapted from https://github.com/imneme/pcg-c-basic/blob/master/pcg_basic.c
namespace pokesim::internal {
// Generate a uniformly distributed 32-bit random number
constexpr types::rngResult nextRandomValue(types::rngState& state) {
  // NOLINTBEGIN
  types::rngState oldState = state;
  state = oldState * 6364136223846793005ULL;
  types::rngResult xorShifted = (types::rngResult)(((oldState >> 18U) ^ oldState) >> 27U);
  types::rngResult rot = oldState >> 59U;
  return (xorShifted >> rot) | (xorShifted << ((-1 * rot) & 31));
  // NOLINTEND
}

// Generate a uniformly distributed 32-bit random number
inline types::rngResult nextRandomValue(RngSeed& seed) {
  return nextRandomValue(seed.val);
}

// Generate a uniformly distributed number, r, where 0 <= r < bound
inline types::rngResult nextBoundedRandomValue(types::rngState& state, types::rngResult upperBound) {
  // To avoid bias, we need to make the range of the RNG a multiple of
  // bound, which we do by dropping output less than a threshold.
  // A naive scheme to calculate the threshold would be to do
  //
  //     uint32_t threshold = 0x100000000ull % bound;
  //
  // but 64-bit div/mod is slower than 32-bit div/mod (especially on
  // 32-bit platforms).  In essence, we do
  //
  //     uint32_t threshold = (0x100000000ull-bound) % bound;
  //
  // because this version will calculate the same modulus, but the LHS
  // value is less than 2^32.

  types::rngResult threshold = (-1 * upperBound) % upperBound;

  // Uniformity guarantees that this loop will terminate.  In practice, it
  // should usually terminate quickly; on average (assuming all bounds are
  // equally likely), 82.25% of the time, we can expect it to require just
  // one iteration.  In the worst case, someone passes a bound of 2^31 + 1
  // (i.e., 2147483649), which invalidates almost 50% of the range.  In
  // practice, bounds are typically small and only a tiny amount of the range
  // is eliminated.
  for (;;) {
    types::rngResult value = nextRandomValue(state);
    if (value >= threshold) return value % upperBound;
  }
}

// Generate a uniformly distributed number, r, where lower bound <= r < upper bound
inline types::rngResult nextBoundedRandomValue(
  types::rngState& state, types::rngResult upperBound, types::rngResult lowerBound) {
  return nextBoundedRandomValue(state, upperBound - lowerBound) + lowerBound;
}
}  // namespace pokesim::internal

namespace pokesim::internal {
// Generate a uniformly distributed number, r, where 0 <= r < bound
inline types::rngResult nextBoundedRandomValue(RngSeed& seed, types::rngResult upperBound) {
  return nextBoundedRandomValue(seed.val, upperBound);
}

// Generate a uniformly distributed number, r, where lower bound <= r < upper bound
inline types::rngResult nextBoundedRandomValue(
  RngSeed& seed, types::rngResult upperBound, types::rngResult lowerBound) {
  return nextBoundedRandomValue(seed.val, upperBound, lowerBound);
}

template <typename List>
constexpr void shuffleShortList(
  List& list, types::eventPossibilities shuffleStart, types::eventPossibilities shuffleLength,
  types::eventPossibilities shuffleOrderIndex) {
  // This loop shuffles a list based on one random number where each possible shuffling has an equal chance. The main
  // idea is to choose where each action should be placed in that new ordering by using the ranges the random number
  // belongs to. For example, let's say there are 3 actions that speed tied, the random number (randomEventIndex) is 2,
  // and we want to find where the third action in the original ordering should move to. There are 6 different ways to
  // order those actions (3 factorial = 6), meaning the randomEventIndex could include any value from 0 to 5. The code
  // below first separates the 0 to 5 range into 3 equally sized sections (so 2 values per section) and then calculates
  // that the randomEventIndex of 2 is in the second section. That section's index is used as where the third action
  // will go as there will be as many sections as there are places to move that action.

  types::eventPossibilities valuesPerSection = 1U;
  for (types::eventPossibilities sections = 1U; sections < shuffleLength; sections++) {
    types::eventPossibilities totalValues = (sections + 1U) * valuesPerSection;
    types::eventPossibilities eventIndexInAllValues = shuffleOrderIndex % totalValues;
    types::eventPossibilities sectionOfEventIndex = eventIndexInAllValues / valuesPerSection;

    types::eventPossibilities oldIndex = shuffleStart + sections;
    types::eventPossibilities newIndex = shuffleStart + sectionOfEventIndex;

    POKESIM_REQUIRE(newIndex < shuffleStart + shuffleLength, "newIndex would be out of bounds for this shuffle.");

    std::swap(list[oldIndex], list[newIndex]);
    valuesPerSection = totalValues;
  }
}
}  // namespace pokesim::internal
