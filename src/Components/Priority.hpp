#pragma once

#include <Types/Constants.hpp>
#include <Types/Move.hpp>

namespace pokesim {
struct MovePriority {
  types::priority val = Constants::MovePriority::DEFAULT;
};
}  // namespace pokesim
