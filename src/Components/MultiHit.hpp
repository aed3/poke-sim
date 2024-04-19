#pragma once

#include <Types/Move.hpp>

namespace pokesim {
// The minimum and maximum number of hits a multi-hit move can cause
struct MultiHit {
  types::moveHits minHits = 2;
  types::moveHits maxHits = 5;
};

struct HitCount {
  types::moveHits hitCount = 1;
};
}  // namespace pokesim