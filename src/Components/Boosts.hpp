#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
struct AtkBoost {
  types::boost val = MechanicConstants::PokemonStatBoost::BASE;
};

struct DefBoost {
  types::boost val = MechanicConstants::PokemonStatBoost::BASE;
};

struct SpaBoost {
  types::boost val = MechanicConstants::PokemonStatBoost::BASE;
};

struct SpdBoost {
  types::boost val = MechanicConstants::PokemonStatBoost::BASE;
};

struct SpeBoost {
  types::boost val = MechanicConstants::PokemonStatBoost::BASE;
};
}  // namespace pokesim