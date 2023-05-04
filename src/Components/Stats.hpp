#pragma once

#include <cstdint>

namespace pokesim::stat {
struct HP {
  std::uint16_t hp = 1;
};

struct Atk {
  std::uint16_t atk = 1;
};

struct Def {
  std::uint16_t def = 1;
};

struct Spa {
  std::uint16_t spa = 1;
};

struct Spd {
  std::uint16_t spd = 1;
};

struct Spe {
  std::uint16_t spe = 1;
};
}  // namespace pokesim::stat
