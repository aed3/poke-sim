#pragma once

#include <cstdint>

namespace pokesim {
struct EVs {
  std::uint8_t hp = 0;
  std::uint8_t atk = 0;
  std::uint8_t def = 0;
  std::uint8_t spa = 0;
  std::uint8_t spd = 0;
  std::uint8_t spe = 0;
};

struct IVs {
  std::uint8_t hp = 0;
  std::uint8_t atk = 0;
  std::uint8_t def = 0;
  std::uint8_t spa = 0;
  std::uint8_t spd = 0;
  std::uint8_t spe = 0;
};
}  // namespace pokesim
