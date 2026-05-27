#pragma once

#include <Utilities/NumberToType.hpp>

#include "Constants.hpp"

namespace pokesim::types {
using level = pokesim::internal::unsignedIntType<Constants::PokemonLevel::MAX>;

using stat = pokesim::internal::unsignedIntType<Constants::PokemonEffectiveStat::MAX>;
using baseStat = pokesim::internal::unsignedIntType<Constants::PokemonBaseStat::MAX>;

using ev = pokesim::internal::unsignedIntType<Constants::PokemonEv::MAX>;
using iv = pokesim::internal::unsignedIntType<Constants::PokemonIv::MAX>;

using boost = pokesim::internal::signedIntType<Constants::PokemonStatBoost::MAX, Constants::PokemonStatBoost::MIN>;
}  // namespace pokesim::types
