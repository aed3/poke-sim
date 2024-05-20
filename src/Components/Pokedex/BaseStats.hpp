#pragma once

#include <Types/Stats.hpp>

namespace pokesim {
// Contains all of the base stats of a species
struct BaseStats {
  types::baseStat hp = 1;
  types::baseStat atk = 1;
  types::baseStat def = 1;
  types::baseStat spa = 1;
  types::baseStat spd = 1;
  types::baseStat spe = 1;
};
}  // namespace pokesim
