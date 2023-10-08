#pragma once

#include <cstdint>

namespace pokesim::stat {
struct HP {
  std::uint16_t stat = 1;
};

struct Atk {
  std::uint16_t stat = 1;
};

struct Def {
  std::uint16_t stat = 1;
};

struct Spa {
  std::uint16_t stat = 1;
};

struct Spd {
  std::uint16_t stat = 1;
};

struct Spe {
  std::uint16_t stat = 1;
};
}  // namespace pokesim::stat
