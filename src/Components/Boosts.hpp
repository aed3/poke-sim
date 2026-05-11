#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
struct AtkBoost {
  types::boost val = MechanicConstants::PokemonStatBoost::DEFAULT;
};

struct DefBoost {
  types::boost val = MechanicConstants::PokemonStatBoost::DEFAULT;
};

struct SpaBoost {
  types::boost val = MechanicConstants::PokemonStatBoost::DEFAULT;
};

struct SpdBoost {
  types::boost val = MechanicConstants::PokemonStatBoost::DEFAULT;
};

struct SpeBoost {
  types::boost val = MechanicConstants::PokemonStatBoost::DEFAULT;
};
}  // namespace pokesim
