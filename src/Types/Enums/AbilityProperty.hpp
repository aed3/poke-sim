#pragma once

#include <cstdint>

namespace pokesim::dex {
enum class AbilityProperty : std::uint8_t {
  CANNOT_BREAK = 1U << 0U,
  CANNOT_SUPPRESS = 1U << 1U,
  NO_ROLE_PLAY = 1U << 2U,
  NO_SKILL_SWAP = 1U << 3U,
  NO_ENTRAINMENT = 1U << 4U,
  NO_RECEIVER = 1U << 5U,
  NO_TRACE = 1U << 6U,
  NO_TRANSFORM = 1U << 7U,
};

constexpr AbilityProperty operator|(AbilityProperty kindA, AbilityProperty kindB) {
  return static_cast<AbilityProperty>(static_cast<std::uint8_t>(kindA) | static_cast<std::uint8_t>(kindB));
}

constexpr bool operator&(AbilityProperty kindA, AbilityProperty kindB) {
  return (static_cast<std::uint8_t>(kindA) & static_cast<std::uint8_t>(kindB)) != 0U;
}
}  // namespace pokesim::dex
