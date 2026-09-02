#pragma once

#include <array>
#include <cstdint>

namespace pokesim {
enum class Slot : std::uint8_t {
  NONE = 0U,
  P1A = 0x10,
  P1B = 0x11,
  P1C = 0x12,
  P1D = 0x13,
  P1E = 0x14,
  P1F = 0x15,

  P2A = 0x20,
  P2B = 0x21,
  P2C = 0x22,
  P2D = 0x23,
  P2E = 0x24,
  P2F = 0x25,
};

namespace internal {
static constexpr inline std::array<Slot, 13U> VALID_SLOTS = {
  Slot::P1A,
  Slot::P1B,
  Slot::P1C,
  Slot::P1D,
  Slot::P1E,
  Slot::P1F,

  Slot::P2A,
  Slot::P2B,
  Slot::P2C,
  Slot::P2D,
  Slot::P2E,
  Slot::P2F,
};

static constexpr std::uint8_t SLOT_SIDE_MASK = 0xF0;
static constexpr std::uint8_t SLOT_LETTER_MASK = 0x0F;
}  // namespace internal
}  // namespace pokesim
