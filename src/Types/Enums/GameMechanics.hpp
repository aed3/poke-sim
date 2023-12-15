#pragma once

#include <cstdint>

namespace pokesim {
// Game the simulator is imitating the mechanics of
enum class GameMechanics : std::uint8_t {
  NONE = 0,
  SWORD_SHIELD,
  BRILLIANT_DIAMOND_SHINING_PEARL,
  SCARLET_VIOLET,
};
}  // namespace pokesim