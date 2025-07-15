#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/State.hpp>

namespace pokesim {
// The position of a Pokemon in its team's order (starts at 1)
struct Position {
  types::teamPositionIndex val = MechanicConstants::TeamSize::MIN;
};
}  // namespace pokesim