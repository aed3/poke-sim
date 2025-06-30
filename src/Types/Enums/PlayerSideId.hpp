#pragma once
#include <cstdint>

namespace pokesim {
enum class PlayerSideId : std::uint8_t {
  NONE = 0,
  P1 = 1,
  P2 = 2,
};

static constexpr std::uint8_t TOTAL_PLAYER_SIDE_ID_COUNT = 2U;
}  // namespace pokesim