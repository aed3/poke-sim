#pragma once

#include <array>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace pokesim {
enum class ActionOrder : std::uint8_t {
  NONE = std::numeric_limits<std::underlying_type_t<ActionOrder>>::max(),
  TEAM = 1U,
  START = 2U,
  MID_TURN_SWITCH = 3U,
  BEFORE_TURN = 4U,
  ITEM = BEFORE_TURN,

  SWITCH = 103U,

  MOVE = 200U,

  RESIDUAL = 254U,
};

namespace internal {
static constexpr inline std::array<ActionOrder, 9U> VALID_ACTION_ORDERS = {
  ActionOrder::NONE,
  ActionOrder::TEAM,
  ActionOrder::START,
  ActionOrder::MID_TURN_SWITCH,
  ActionOrder::BEFORE_TURN,
  ActionOrder::ITEM,
  ActionOrder::SWITCH,
  ActionOrder::MOVE,
  ActionOrder::RESIDUAL,
};
}  // namespace internal
}  // namespace pokesim
