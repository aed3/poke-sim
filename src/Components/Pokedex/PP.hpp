#pragma once

#include <Types/Constants.hpp>
#include <Types/Move.hpp>

namespace pokesim {
struct Pp {
  types::pp val = Constants::MovePp::DEFAULT;
};
}  // namespace pokesim
