#pragma once

#include <Types/Random.hpp>
#include <array>
#include <cmath>
#include <cstdint>

namespace pokesim {
struct MechanicConstants {
  static constexpr std::array<types::percentChance, 4U> CRIT_CHANCE_DIVISORS{24U, 8U, 2U, 1U};
  static constexpr std::uint8_t MAX_DAMAGE_ROLL_COUNT = 16U;

  static constexpr std::uint8_t MAX_TEAM_SIZE = 6U;
  static constexpr std::uint8_t MAX_ACTIVE_POKEMON_SLOTS = 2U;
  static constexpr std::uint8_t MAX_MOVE_SLOTS = 4U;
  static constexpr std::uint8_t MAX_TARGETS = 3U;

  static constexpr float CRIT_MULTIPLIER = 1.5F;
  static constexpr float STAB_MULTIPLIER = 1.5F;

  static constexpr std::uint8_t FIXED_POINT_SCALING_FACTOR = 12U;
  static constexpr std::uint16_t FIXED_POINT_SCALE = 1U << FIXED_POINT_SCALING_FACTOR;
  static constexpr std::uint16_t FIXED_POINT_HALF_SCALE = FIXED_POINT_SCALE / 2U;
};
}  // namespace pokesim