#pragma once

#include <cstdint>

namespace pokesim {
// The position of a Pokemon in its team's order (starts at 1)
struct Position {
  std::uint8_t position = 1;
};
}  // namespace pokesim