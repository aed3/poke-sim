#pragma once
#include <cstdint>

namespace pokesim {
enum class PlayerSideId : std::uint8_t {
  NONE = 0U,
  P1 = 1U,
  P2 = 2U,
};

static constexpr std::uint8_t TOTAL_PLAYER_SIDE_ID_COUNT = 2U;
}  // namespace pokesim