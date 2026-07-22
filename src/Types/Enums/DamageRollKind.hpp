#pragma once

#include <array>
#include <cstdint>

namespace pokesim {
enum class DamageRollKind : std::uint8_t {
  NONE = 0U,
  AVERAGE_DAMAGE = 1U << 0U,
  MAX_DAMAGE = 1U << 1U,
  MIN_DAMAGE = 1U << 2U,
  GUARANTEED_CRIT_CHANCE = 1U << 3U,
  ALL_DAMAGE_ROLLS = 1U << 4U,
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
