#pragma once
#include <cstdint>

namespace pokesim {
enum class PlayerSideId : std::uint8_t {
  NONE = 0,
  P1 = 1,
  P2 = 2,
};
}