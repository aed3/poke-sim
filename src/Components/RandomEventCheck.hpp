#pragma once

#include <Types/Random.hpp>
#include <Utilities/MaxSizedVector.hpp>
#include <cstdint>

namespace pokesim {
namespace internal {
// Moves such as Metronome and Psywave, the Forewarn ability, and damage rolls have more random options than 4, but
// those cases will be handled separately
const std::uint8_t MAX_TYPICAL_RANDOM_OPTIONS = 5U;
}  // namespace internal

template <std::uint8_t RANDOM_OPTIONS>
struct RandomEventCheck {
  internal::maxSizedVector<types::percentChance, RANDOM_OPTIONS> chances;
};

struct RandomBinaryEventCheck {
  types::percentChance checkPassChance = 100;
};
}  // namespace pokesim