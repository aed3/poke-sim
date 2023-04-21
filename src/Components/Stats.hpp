#pragma once

#include <cstdint>

namespace pokesim::stat {
struct HP {
  std::uint16_t hp;
};

struct Atk {
  std::uint16_t atk;
};

struct Def {
  std::uint16_t def;
};

struct Spa {
  std::uint16_t spa;
};

struct Spd {
  std::uint16_t spd;
};

struct Spe {
  std::uint16_t spe;
};
}  // namespace pokesim::stat
