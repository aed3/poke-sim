#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
// Contains all of the base stats of a species
struct BaseStats {
  types::baseStat hp = MechanicConstants::PokemonBaseStat::MIN;
  types::baseStat atk = MechanicConstants::PokemonBaseStat::MIN;
  types::baseStat def = MechanicConstants::PokemonBaseStat::MIN;
  types::baseStat spa = MechanicConstants::PokemonBaseStat::MIN;
  types::baseStat spd = MechanicConstants::PokemonBaseStat::MIN;
  types::baseStat spe = MechanicConstants::PokemonBaseStat::MIN;
};
}  // namespace pokesim
