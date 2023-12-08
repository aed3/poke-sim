#pragma once

#include <Types/Stats.hpp>

namespace pokesim {
// Contains all of the base stats of a species
struct BaseStats {
  types::BaseStat hp = 1;
  types::BaseStat atk = 1;
  types::BaseStat def = 1;
  types::BaseStat spa = 1;
  types::BaseStat spd = 1;
  types::BaseStat spe = 1;
};
}  // namespace pokesim
