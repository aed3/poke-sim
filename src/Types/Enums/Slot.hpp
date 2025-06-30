#pragma once

#include <cstdint>

namespace pokesim {
enum class Slot : std::uint8_t {
  NONE = 0,
  P1A,
  P2A,
  P1B,
  P2B,

  P1C,
  P2C,
  P1D,
  P2D,
  P1E,
  P2E,
  P1F,
  P2F,

  TOTAL_SLOT,
};

static constexpr std::uint8_t TOTAL_SLOT_COUNT = (std::uint8_t)Slot::TOTAL_SLOT - 1U;
}  // namespace pokesim