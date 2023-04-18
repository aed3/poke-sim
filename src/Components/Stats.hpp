#pragma once

#include <cstdint>

namespace pokesim {
struct Stats {
  std::uint16_t hp;
  std::uint16_t atk;
  std::uint16_t def;
  std::uint16_t spa;
  std::uint16_t spd;
  std::uint16_t spe;
};
}  // namespace pokesim
