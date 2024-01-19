#pragma once

#include <Types/Stats.hpp>

namespace pokesim {
struct Evs {
  types::ev hp = 0;
  types::ev atk = 0;
  types::ev def = 0;
  types::ev spa = 0;
  types::ev spd = 0;
  types::ev spe = 0;
};

struct Ivs {
  types::iv hp = 0;
  types::iv atk = 0;
  types::iv def = 0;
  types::iv spa = 0;
  types::iv spd = 0;
  types::iv spe = 0;
};
}  // namespace pokesim
