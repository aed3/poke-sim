#pragma once

#include <Types/Random.hpp>
#include <Types/State.hpp>
#include <array>
#include <cstdint>

namespace pokesim {
namespace internal {
// Moves such as Metronome and Psywave, the Forewarn ability, and damage rolls have more random options than 4, but
// those cases will be handled separately
const std::uint8_t MAX_TYPICAL_RANDOM_OPTIONS = 5U;
}  // namespace internal

template <std::uint8_t RANDOM_OPTIONS>
struct RandomEventChances {
  std::array<types::percentChance, RANDOM_OPTIONS> val{};
};

struct RandomBinaryEventChance {
  types::percentChance val = 100;
};

template <std::uint8_t RANDOM_OPTIONS>
struct RandomEventChancesStack {
  types::targets<RandomEventChances<RANDOM_OPTIONS>> val{};
};

struct RandomBinaryEventChanceStack {
  types::targets<RandomBinaryEventChance> val{};
};
}  // namespace pokesim