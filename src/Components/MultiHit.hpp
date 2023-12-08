#pragma once

#include <Types/Move.hpp>

namespace pokesim {
// The minimum and maximum number of hits a multi-hit move can cause
struct MultiHit {
  types::MoveHits minHits = 2;
  types::MoveHits maxHits = 5;
};
}  // namespace pokesim