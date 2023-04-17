#pragma once

#include <cstdint>

namespace pokesim {
struct MultiHit {
  std::uint8_t minHits;
  std::uint8_t maxHits;
};
}  // namespace pokesim