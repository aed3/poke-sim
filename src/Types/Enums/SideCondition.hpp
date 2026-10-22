#pragma once

#include <cstdint>

namespace pokesim::dex {
enum class SideCondition : std::uint8_t {
  NO_SIDE_CONDITION = 0U,
  AURORA_VEIL,
  CRAFTY_SHIELD,
  FIRE_PLEDGE,
  GRASS_PLEDGE,
  LIGHT_SCREEN,
  LUCKY_CHANT,
  MAT_BLOCK,
  MIST,
  QUICK_GUARD,
  REFLECT,
  SAFEGUARD,
  SPIKES,
  STEALTH_ROCK,
  STICKY_WEB,
  TAILWIND,
  TOXIC_SPIKES,
  WATER_PLEDGE,
  WIDE_GUARD,

  SIDE_CONDITION_TOTAL,
};

static constexpr std::uint8_t TOTAL_SIDE_CONDITION_COUNT = (std::uint8_t)SideCondition::SIDE_CONDITION_TOTAL - 1U;
}  // namespace pokesim::dex
