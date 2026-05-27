#pragma once

#include <Types/Constants.hpp>
#include <Types/Random.hpp>

namespace pokesim {
struct Accuracy {
  types::percentChance val = Constants::MoveBaseAccuracy::DEFAULT;
};
}  // namespace pokesim
