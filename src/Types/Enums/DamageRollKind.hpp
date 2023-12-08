#pragma once

#include <cstdint>

namespace pokesim {
enum class DamageRollKind : std::uint8_t {
  NONE = 0b00000000,
  AVERAGE_DAMAGE = 0b00000001,
  ALL_DAMAGE_ROLES = 0b00000010,
  P1_MAX_DAMAGE = 0b00000100,
  P2_MAX_DAMAGE = 0b00001000,
  P1_MIN_DAMAGE = 0b00010000,
  P2_MIN_DAMAGE = 0b00100000,

  MAX_DAMAGE = P1_MAX_DAMAGE | P2_MAX_DAMAGE,
  MIN_DAMAGE = P1_MIN_DAMAGE | P2_MIN_DAMAGE,
};
}  // namespace pokesim