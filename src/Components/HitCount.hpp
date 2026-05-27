#pragma once

#include <Types/Constants.hpp>
#include <Types/Move.hpp>

namespace pokesim {
struct HitCount {
  types::moveHits val = Constants::MoveHits::DEFAULT;
};
}  // namespace pokesim
