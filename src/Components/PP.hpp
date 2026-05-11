#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Move.hpp>

namespace pokesim {
struct Pp {
  types::pp val = MechanicConstants::MovePp::DEFAULT;
};

struct MaxPp {
  types::pp val = MechanicConstants::MoveMaxPp::DEFAULT;
};
}  // namespace pokesim
