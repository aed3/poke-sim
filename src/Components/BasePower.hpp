#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Move.hpp>

namespace pokesim {
struct BasePower {
  types::basePower val = MechanicConstants::MoveBasePower::MIN;
};
}  // namespace pokesim