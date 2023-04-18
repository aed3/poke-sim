#pragma once

#include <cstdint>

namespace pokesim {
struct BaseStats {
  std::uint8_t hp;
  std::uint8_t atk;
  std::uint8_t def;
  std::uint8_t spa;
  std::uint8_t spd;
  std::uint8_t spe;
};
}  // namespace pokesim
