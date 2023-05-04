#pragma once

#include <cstdint>

namespace pokesim {
struct MultiHit {
  std::uint8_t minHits = 2;
  std::uint8_t maxHits = 5;
};
}  // namespace pokesim