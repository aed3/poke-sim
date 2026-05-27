#pragma once

#include <Types/Constants.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
struct AtkBoost {
  types::boost val = Constants::PokemonStatBoost::DEFAULT;
};

struct DefBoost {
  types::boost val = Constants::PokemonStatBoost::DEFAULT;
};

struct SpaBoost {
  types::boost val = Constants::PokemonStatBoost::DEFAULT;
};

struct SpdBoost {
  types::boost val = Constants::PokemonStatBoost::DEFAULT;
};

struct SpeBoost {
  types::boost val = Constants::PokemonStatBoost::DEFAULT;
};
}  // namespace pokesim
