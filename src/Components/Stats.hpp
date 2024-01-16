#pragma once

#include <Types/Stats.hpp>

namespace pokesim::stat {
struct Hp {
  types::stat stat = 1;
};

struct Atk {
  types::stat stat = 1;
};

struct Def {
  types::stat stat = 1;
};

struct Spa {
  types::stat stat = 1;
};

struct Spd {
  types::stat stat = 1;
};

struct Spe {
  types::stat stat = 1;
};
}  // namespace pokesim::stat
