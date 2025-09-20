#pragma once

#include <cstdint>

namespace pokesim {
// Game the simulator is imitating the mechanics of
enum class GameMechanics : std::uint8_t {
  NONE = 0U,
  SWORD_SHIELD = 80U,
  BRILLIANT_DIAMOND_SHINING_PEARL = 83U,
  SCARLET_VIOLET = 90U,

  LATEST = SCARLET_VIOLET,
};

static constexpr std::uint8_t TOTAL_GAME_MECHANICS_COUNT = 3U;
}  // namespace pokesim