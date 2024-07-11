#pragma once

#include <cstdint>

namespace pokesim::dex {
// Pokemon stat abbreviated name
enum class Stat : std::uint8_t {
  HP = 0b000001,
  ATK = 0b000010,
  DEF = 0b000100,
  SPA = 0b001000,
  SPD = 0b010000,
  SPE = 0b100000,
  // SPC = SPA | SPD,
};

static constexpr std::size_t TOTAL_STAT_COUNT = 6U;
}  // namespace pokesim::dex
