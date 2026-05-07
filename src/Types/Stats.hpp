#pragma once

#include <Utilities/NumberToType.hpp>

#include "MechanicConstants.hpp"

namespace pokesim::types {
using level = pokesim::internal::unsignedIntType<MechanicConstants::PokemonLevel::MAX>;

using stat = pokesim::internal::unsignedIntType<MechanicConstants::PokemonEffectiveStat::MAX>;
using baseStat = pokesim::internal::unsignedIntType<MechanicConstants::PokemonBaseStat::MAX>;

using ev = pokesim::internal::unsignedIntType<MechanicConstants::PokemonEv::MAX>;
using iv = pokesim::internal::unsignedIntType<MechanicConstants::PokemonIv::MAX>;

using boost =
  pokesim::internal::signedIntType<MechanicConstants::PokemonStatBoost::MAX, MechanicConstants::PokemonStatBoost::MIN>;
}  // namespace pokesim::types
