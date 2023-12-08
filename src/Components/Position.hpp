#pragma once

#include <Types/State.hpp>

namespace pokesim {
// The position of a Pokemon in its team's order (starts at 1)
struct Position {
  types::TeamPositionIndex position = 1;
};
}  // namespace pokesim