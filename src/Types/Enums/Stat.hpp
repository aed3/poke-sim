#pragma once

#include <cstdint>

namespace pokesim::dex {
// Pokemon stat abbreviated name
enum class Stat : std::uint8_t {
  NONE = 0U,
  HP = 1U << 0U,
  ATK = 1U << 1U,
  DEF = 1U << 2U,
  SPA = 1U << 3U,
  SPD = 1U << 4U,
  SPE = 1U << 5U,
  // SPC = SPA | SPD,
};

static constexpr std::uint8_t TOTAL_STAT_COUNT = 6U;
}  // namespace pokesim::dex
