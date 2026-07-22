#pragma once

#include <Types/Constants.hpp>
#include <Types/Indexes.hpp>

namespace pokesim {
// The position of a Pokemon in its team's order (starts at 1)
struct Position {
  types::teamPositionIndex val = Constants::TeamSize::MIN;
};
}  // namespace pokesim
