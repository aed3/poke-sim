#pragma once

#include <cstdint>

namespace pokesim {
// Chance of an effect occurring
// Examples: Air Slash has a 60% chance to flinch, Harvest has a 50% chance to restore a used berry
struct Chance {
  std::uint8_t chance = 100;
};
}  // namespace pokesim