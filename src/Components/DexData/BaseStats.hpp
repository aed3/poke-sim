#pragma once

#include <cstdint>

namespace pokesim {
// Contains all of the base stats of a species
struct BaseStats {
  std::uint8_t hp = 1;
  std::uint8_t atk = 1;
  std::uint8_t def = 1;
  std::uint8_t spa = 1;
  std::uint8_t spd = 1;
  std::uint8_t spe = 1;
};
}  // namespace pokesim
