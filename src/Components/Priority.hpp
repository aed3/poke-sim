#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Move.hpp>

namespace pokesim {
struct MovePriority {
  types::priority val = MechanicConstants::MovePriority::DEFAULT;
};
}  // namespace pokesim
