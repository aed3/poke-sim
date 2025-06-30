#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Random.hpp>

namespace pokesim {
struct Accuracy {
  types::percentChance val = MechanicConstants::MoveBaseAccuracy::MAX;
};
}  // namespace pokesim