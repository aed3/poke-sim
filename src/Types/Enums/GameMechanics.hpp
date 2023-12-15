#pragma once

#include <cstdint>

namespace pokesim {
// Game the simulator is imitating the mechanics of
enum class GameMechanics : std::uint8_t {
  NONE = 0,
  SCARLET_VIOLET,
  BRILLIANT_DIAMOND_SHINING_PEARL,
};
}  // namespace pokesim