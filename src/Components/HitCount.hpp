#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Move.hpp>

namespace pokesim {
struct HitCount {
  types::moveHits val = MechanicConstants::MoveHits::MIN;
};
}  // namespace pokesim