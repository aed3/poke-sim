#pragma once

#include <cstdint>

namespace pokesim {
enum class DamageRollKind : std::uint8_t {
  NONE = 0b00000000,
  AVERAGE_DAMAGE = 0b000000001,
  MAX_DAMAGE = 0b00000010,
  MIN_DAMAGE = 0b00000100,
  GUARANTEED_CRIT_CHANCE = 0b00001000,
  ALL_DAMAGE_ROLLS = 0b00010000,
};
}  // namespace pokesim