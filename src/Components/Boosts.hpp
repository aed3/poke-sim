#pragma once

#include <Types/Stats.hpp>

namespace pokesim {
struct AtkBoost {
  types::boost val = 0;
};

struct DefBoost {
  types::boost val = 0;
};

struct SpaBoost {
  types::boost val = 0;
};

struct SpdBoost {
  types::boost val = 0;
};

struct SpeBoost {
  types::boost val = 0;
};
}  // namespace pokesim