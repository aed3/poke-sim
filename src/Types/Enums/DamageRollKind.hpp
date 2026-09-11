#pragma once

#include <array>
#include <cstdint>

namespace pokesim {
enum class DamageRollKind : std::uint8_t {
  NONE = 0U,
  AVERAGE_DAMAGE = 1U << 0U,
  MAX_DAMAGE = 1U << 1U,
  MIN_DAMAGE = 1U << 2U,
  // Damage as if all moves will crit. Doesn't bypass Shell Armor, Lucky Chant, and other crit blockers.
  GUARANTEED_CRIT_CHANCE = 1U << 3U,
  // Damage as if every move has no chance to crit. Doesn't bypass Flower Trick, Frost Breath, and other moves that are
  // guaranteed to crit.
  NO_CRIT_CHANCE = 1U << 4U,
  ALL_DAMAGE_ROLLS = 1U << 5U,
};

constexpr DamageRollKind operator|(DamageRollKind kindA, DamageRollKind kindB) {
  return static_cast<DamageRollKind>(static_cast<std::uint8_t>(kindA) | static_cast<std::uint8_t>(kindB));
}

constexpr bool operator&(DamageRollKind kindA, DamageRollKind kindB) {
  return (static_cast<std::uint8_t>(kindA) & static_cast<std::uint8_t>(kindB)) != 0U;
}

namespace internal {
static constexpr inline std::array<DamageRollKind, 7U> VALID_DAMAGE_ROLL_KINDS = {
  DamageRollKind::NONE,
  DamageRollKind::AVERAGE_DAMAGE,
  DamageRollKind::MAX_DAMAGE,
  DamageRollKind::MIN_DAMAGE,
  DamageRollKind::GUARANTEED_CRIT_CHANCE,
  DamageRollKind::NO_CRIT_CHANCE,
  DamageRollKind::ALL_DAMAGE_ROLLS,
};
}  // namespace internal
}  // namespace pokesim
