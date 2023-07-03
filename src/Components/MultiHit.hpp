#pragma once

#include <cstdint>

namespace pokesim {
// The minimum and maximum number of hits a multi-hit move can cause
struct MultiHit {
  std::uint8_t minHits = 2;
  std::uint8_t maxHits = 5;
};
}  // namespace pokesim