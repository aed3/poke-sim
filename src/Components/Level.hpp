#pragma once

#include <Types/Constants.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
// A Pokemon's level
struct Level {
  types::level val = Constants::PokemonLevel::DEFAULT;
};
}  // namespace pokesim
