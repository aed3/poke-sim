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
  static_assert(RANDOM_OPTIONS <= 5);

  types::percentChance chanceA() const { return val[0]; }
  types::percentChance chanceB() const { return val[1] - val[0]; }
  types::percentChance chanceC() const { return val[2] - val[1]; }
  types::percentChance chanceD() const {
    static_assert(RANDOM_OPTIONS >= 4);
    return val[3] - val[2];
  }
  types::percentChance chanceE() const {
    static_assert(RANDOM_OPTIONS == 5);
    return val[4] - val[3];
  }
};

struct RandomBinaryEventChance {
  types::percentChance val = 100;

  types::percentChance pass() const { return val; }
  types::percentChance fail() const { return 100 - pass(); }
  float reciprocalPass() const { return 100.0 / pass(); }
  float reciprocalFail() const { return 100 - reciprocalPass(); }
};

template <std::uint8_t RANDOM_OPTIONS>
struct RandomEventChancesStack {
  types::targets<RandomEventChances<RANDOM_OPTIONS>> val{};
};

struct RandomBinaryEventChanceStack {
  types::targets<RandomBinaryEventChance> val{};
};
}  // namespace pokesim