#pragma once

#include <Types/Constants.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
// Contains all the base stats of a species
struct BaseStats {
  types::baseStat hp = Constants::PokemonBaseStat::DEFAULT;
  types::baseStat atk = Constants::PokemonBaseStat::DEFAULT;
  types::baseStat def = Constants::PokemonBaseStat::DEFAULT;
  types::baseStat spa = Constants::PokemonBaseStat::DEFAULT;
  types::baseStat spd = Constants::PokemonBaseStat::DEFAULT;
  types::baseStat spe = Constants::PokemonBaseStat::DEFAULT;
};
}  // namespace pokesim
