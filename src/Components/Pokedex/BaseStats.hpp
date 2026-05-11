#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
// Contains all of the base stats of a species
struct BaseStats {
  types::baseStat hp = MechanicConstants::PokemonBaseStat::DEFAULT;
  types::baseStat atk = MechanicConstants::PokemonBaseStat::DEFAULT;
  types::baseStat def = MechanicConstants::PokemonBaseStat::DEFAULT;
  types::baseStat spa = MechanicConstants::PokemonBaseStat::DEFAULT;
  types::baseStat spd = MechanicConstants::PokemonBaseStat::DEFAULT;
  types::baseStat spe = MechanicConstants::PokemonBaseStat::DEFAULT;
};
}  // namespace pokesim
