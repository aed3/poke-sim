#pragma once

#include <array>
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

  RESIDUAL = 254,
};

static constexpr inline std::array<ActionOrder, 8U> VALID_ACTION_ORDERS = {
  ActionOrder::NONE,
  ActionOrder::TEAM,
  ActionOrder::START,
  ActionOrder::BEFORE_TURN,
  ActionOrder::ITEM,
  ActionOrder::SWITCH,
  ActionOrder::MOVE,
  ActionOrder::RESIDUAL,
};
}  // namespace pokesim