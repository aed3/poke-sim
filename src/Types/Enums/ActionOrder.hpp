#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>

namespace pokesim {
enum class ActionOrder : std::uint8_t {
  NONE = std::numeric_limits<std::underlying_type_t<ActionOrder>>::max(),
  TEAM = 1,
  START = 2,
  BEFORE_TURN = 4,
  ITEM = BEFORE_TURN,

  SWITCH = 103,

  MOVE = 200,
};
}