#pragma once

#include <array>
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

constexpr DamageRollKind operator|(DamageRollKind kindA, DamageRollKind kindB) {
  return static_cast<DamageRollKind>(static_cast<std::uint8_t>(kindA) | static_cast<std::uint8_t>(kindB));
}

constexpr bool operator&(DamageRollKind kindA, DamageRollKind kindB) {
  return (static_cast<std::uint8_t>(kindA) & static_cast<std::uint8_t>(kindB)) != 0U;
}

static constexpr inline std::array<DamageRollKind, 6U> VALID_DAMAGE_ROLL_KINDS = {
  DamageRollKind::NONE,
  DamageRollKind::AVERAGE_DAMAGE,
  DamageRollKind::MAX_DAMAGE,
  DamageRollKind::MIN_DAMAGE,
  DamageRollKind::GUARANTEED_CRIT_CHANCE,
  DamageRollKind::ALL_DAMAGE_ROLLS,
};
}  // namespace pokesim