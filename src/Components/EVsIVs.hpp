#pragma once

#include <Types/Stats.hpp>

namespace pokesim {
struct Evs {
  types::Ev hp = 0;
  types::Ev atk = 0;
  types::Ev def = 0;
  types::Ev spa = 0;
  types::Ev spd = 0;
  types::Ev spe = 0;
};

struct Ivs {
  types::Iv hp = 0;
  types::Iv atk = 0;
  types::Iv def = 0;
  types::Iv spa = 0;
  types::Iv spd = 0;
  types::Iv spe = 0;
};
}  // namespace pokesim
