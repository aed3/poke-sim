#pragma once

#include <cstdint>

namespace pokesim::dex {
// Pokemon type name
enum class Type : std::uint8_t {
  NO_TYPE = 0,
  NORMAL,
  GRASS,
  WATER,
  FIRE,
  BUG,
  FLYING,
  POISON,
  ELECTRIC,
  GROUND,
  FIGHTING,
  PSYCHIC,
  ROCK,
  ICE,
  GHOST,
  DRAGON,
  DARK,
  STEEL,
  FAIRY,
  TYPE_TOTAL
};

static constexpr std::uint8_t TOTAL_TYPE_COUNT = (std::uint8_t)Type::TYPE_TOTAL - 1U;
}  // namespace pokesim::dex
