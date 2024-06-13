#pragma once

#include <Components/RNGSeed.hpp>
#include <Types/Random.hpp>

// Adapted from https://github.com/imneme/pcg-c-basic/blob/master/pcg_basic.c
namespace pokesim::internal {

// Generate a uniformly distributed 32-bit random number
inline types::rngResult nextRandomValue(types::rngState& state) {
  // NOLINTBEGIN
  types::rngState oldState = state;
  state = oldState * 6364136223846793005ULL;
  types::rngResult xorShifted = ((oldState >> 18U) ^ oldState) >> 27U;
  types::rngResult rot = oldState >> 59U;
  return (xorShifted >> rot) | (xorShifted << ((-rot) & 31));
  // NOLINTEND
}

// Generate a uniformly distributed 32-bit random number
inline types::rngResult nextRandomValue(RngSeed& seed) {
  return nextRandomValue(seed.seed);
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

  types::rngResult threshold = -upperBound % upperBound;

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

// Generate a uniformly distributed number, r, where 0 <= r < bound
inline types::rngResult nextBoundedRandomValue(RngSeed& seed, types::rngResult upperBound) {
  return nextBoundedRandomValue(seed.seed, upperBound);
}
}  // namespace pokesim::internal