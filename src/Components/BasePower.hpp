#pragma once

#include <Types/Constants.hpp>
#include <Types/Move.hpp>

namespace pokesim {
struct BasePower {
  types::basePower val = Constants::MoveBasePower::DEFAULT;
};
}  // namespace pokesim
