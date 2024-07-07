#pragma once

#include <cstdint>

namespace pokesim {
enum class DamageRollKind : std::uint8_t {
  NONE = 0b00000000,
  AVERAGE_DAMAGE = 0b00000001,
  ALL_DAMAGE_ROLES = 0b00000010,
  MAX_DAMAGE = 0b00000100,
  MIN_DAMAGE = 0b00001000,
};
}  // namespace pokesim