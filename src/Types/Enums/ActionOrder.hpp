#pragma once
#include <cstdint>

namespace pokesim {
enum class ActionOrder : std::uint8_t {
  NONE = 0,
  TEAM = 1,
  START = 2,
  BEFORE_TURN = 4,
  ITEM = BEFORE_TURN,

  SWITCH = 103,

  MOVE = 200,
};
}