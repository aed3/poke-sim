#pragma once

#include "MechanicConstants.hpp"
#include "NumberToType.hpp"

namespace pokesim::types {
using level = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::POKEMON_LEVEL>;

using stat = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::POKEMON_STAT>;
using baseStat = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::POKEMON_BASE_STAT>;

using ev = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::POKEMON_EV>;
using iv = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::POKEMON_IV>;

using boost = pokesim::internal::signedIntType<
  MechanicConstants::MaxValues::POKEMON_STAT_BOOST, MechanicConstants::MinValues::POKEMON_STAT_BOOST>;
}  // namespace pokesim::types