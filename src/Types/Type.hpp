#pragma once

#include <cstdint>

namespace pokesim::dex {
/** @brief Pokemon type name */
enum Type : std::uint8_t {
  // clang-format off
  NO_TYPE = 0, NORMAL_TYPE, FIGHTING_TYPE, FLYING_TYPE, POISON_TYPE, GROUND_TYPE, ROCK_TYPE, BUG_TYPE, GHOST_TYPE, STEEL_TYPE, FIRE_TYPE, WATER_TYPE, GRASS_TYPE, ELECTRIC_TYPE, PSYCHIC_TYPE, ICE_TYPE, DRAGON_TYPE, DARK_TYPE, FAIRY_TYPE, TYPE_TOTAL
  // clang-format on
};
}  // namespace pokesim::dex
