#pragma once

#include <Types/Stats.hpp>

namespace pokesim::stat {
struct Hp {
  types::Stat stat = 1;
};

struct Atk {
  types::Stat stat = 1;
};

struct Def {
  types::Stat stat = 1;
};

struct Spa {
  types::Stat stat = 1;
};

struct Spd {
  types::Stat stat = 1;
};

struct Spe {
  types::Stat stat = 1;
};
}  // namespace pokesim::stat
