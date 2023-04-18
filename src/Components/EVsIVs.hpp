#pragma once

#include <cstdint>

namespace pokesim {
struct EVs {
  std::uint8_t hp;
  std::uint8_t atk;
  std::uint8_t def;
  std::uint8_t spa;
  std::uint8_t spd;
  std::uint8_t spe;
};

struct IVs {
  std::uint8_t hp;
  std::uint8_t atk;
  std::uint8_t def;
  std::uint8_t spa;
  std::uint8_t spd;
  std::uint8_t spe;
};
}  // namespace pokesim
