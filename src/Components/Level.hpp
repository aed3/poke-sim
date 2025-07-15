#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
// A Pokemon's level
struct Level {
  types::level val = MechanicConstants::PokemonLevel::MIN;
};
}  // namespace pokesim