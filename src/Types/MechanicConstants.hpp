#pragma once

#include <Types/Random.hpp>
#include <array>
#include <cstdint>

namespace pokesim {
struct MechanicConstants {
  static constexpr std::uint8_t TYPES_PER_POKEMON = 2U;

  static constexpr std::array<types::percentChance, 4U> CRIT_CHANCE_DIVISORS{24U, 8U, 2U, 1U};

  // The 35%-35%-15%-15% out of 100 for 2-3-4-5 hits added so each index is the sum of the chance of its hit count and
  // the hit counts less than it so it works with the randomEventChances function
  static constexpr std::array<types::percentChance, 4U> PROGRESSIVE_MULTI_HIT_CHANCES{35U, 70U, 85U, 100U};

  // TODO(aed3): Should change based on simulation's game mechanics
  static constexpr float CRIT_MULTIPLIER = 1.5F;

  static constexpr std::uint8_t FIXED_POINT_SCALING_FACTOR = 12U;
  static constexpr std::uint16_t FIXED_POINT_SCALE = 1U << FIXED_POINT_SCALING_FACTOR;
  static constexpr std::uint16_t FIXED_POINT_HALF_SCALE = FIXED_POINT_SCALE / 2U;

  static constexpr std::uint8_t SIDE_COUNT = 2U;

  struct MaxValues {
    static constexpr std::uint8_t POKEMON_LEVEL = 100U;
    static constexpr std::uint8_t POKEMON_BASE_STAT = 255U;
    static constexpr std::uint16_t POKEMON_STAT = 65535U;
    static constexpr std::uint8_t POKEMON_EV = 255U;
    static constexpr std::uint8_t POKEMON_IV = 31U;
    static constexpr std::int8_t POKEMON_STAT_BOOST = 6;

    static constexpr std::uint8_t MOVE_MAX_PP = 64U;
    static constexpr std::uint8_t MOVE_PP = MOVE_MAX_PP;
    static constexpr std::uint8_t MOVE_BASE_POWER = 255U;
    static constexpr std::uint8_t MOVE_BASE_ACCURACY = 100U;
    static constexpr std::uint8_t MOVE_HITS = 10U;
    static constexpr std::uint8_t MOVE_BASE_EFFECT_CHANCE = 100U;
    static constexpr std::int8_t MOVE_PRIORITY =
      5;  // 8 is theoretically possible, but no existing move has more than 5

    static constexpr std::uint16_t DAMAGE = 65535U;
    static constexpr std::uint8_t DAMAGE_ROLL_COUNT = 16U;

    static constexpr std::int8_t TYPE_EFFECTIVENESS_SHIFT = 3;

    static constexpr std::uint8_t TEAM_SIZE = 6U;
    static constexpr std::uint8_t ACTIVE_POKEMON_SLOTS_PER_SIDE = 2U;
    static constexpr std::uint8_t ACTIVE_POKEMON = ACTIVE_POKEMON_SLOTS_PER_SIDE * SIDE_COUNT;
    static constexpr std::uint8_t MOVE_SLOTS = 4U;
    static constexpr std::uint8_t TARGETS = 3U;

    static constexpr float PROBABILITY = 1.0F;

    // TODO(aed3): 64 is a guess, so find out what the actual number is
    static constexpr std::uint8_t ACTION_QUEUE_LENGTH = 64U;

    // TODO(aed3): Technically 65535, but battles over 1000 turns aren't usually supported on Showdown
    static constexpr std::uint16_t TURN_COUNT = 1000U;
  };

  struct MinValues {
    static constexpr std::uint8_t POKEMON_LEVEL = 1U;
    static constexpr std::uint8_t POKEMON_BASE_STAT = 1U;
    static constexpr std::uint16_t POKEMON_STAT = 1U;
    static constexpr std::uint8_t POKEMON_EV = 0U;
    static constexpr std::uint8_t POKEMON_IV = 0U;
    static constexpr std::int8_t POKEMON_STAT_BOOST = -6;

    static constexpr std::uint8_t MOVE_MAX_PP = 1U;
    static constexpr std::uint8_t MOVE_PP = 0U;
    static constexpr std::uint8_t MOVE_BASE_POWER = 1U;
    static constexpr std::uint8_t MOVE_BASE_ACCURACY = 1U;
    static constexpr std::uint8_t MOVE_HITS = 1U;
    static constexpr std::uint8_t MOVE_BASE_EFFECT_CHANCE = 1U;
    static constexpr std::int8_t MOVE_PRIORITY = -7;

    static constexpr std::uint16_t DAMAGE = 1U;
    static constexpr std::uint8_t DAMAGE_ROLL_COUNT = 1U;

    static constexpr std::int8_t TYPE_EFFECTIVENESS_SHIFT = -7;

    static constexpr std::uint8_t TEAM_SIZE = 1U;
    static constexpr std::uint8_t ACTIVE_POKEMON_SLOTS_PER_SIDE = 1U;
    static constexpr std::uint8_t ACTIVE_POKEMON = 0U;
    static constexpr std::uint8_t MOVE_SLOTS = 1U;
    static constexpr std::uint8_t TARGETS = 1U;

    static constexpr float PROBABILITY = 0.0F;

    static constexpr std::uint8_t ACTION_QUEUE_LENGTH = 0U;
    static constexpr std::uint16_t TURN_COUNT = 0U;
  };
};
}  // namespace pokesim