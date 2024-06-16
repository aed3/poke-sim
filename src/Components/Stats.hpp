#pragma once

#include <Types/Stats.hpp>

namespace pokesim::stat {
struct Hp {
  types::stat val = 1;
};

struct Atk {
  types::stat val = 1;
};

struct Def {
  types::stat val = 1;
};

struct Spa {
  types::stat val = 1;
};

struct Spd {
  types::stat val = 1;
};

struct Spe {
  types::stat val = 1;
};

struct EffectiveSpeed {
  types::stat effectiveSpeed = 1;
};
}  // namespace pokesim::stat
