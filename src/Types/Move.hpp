#pragma once

#include "MechanicConstants.hpp"
#include "NumberToType.hpp"

namespace pokesim::types {
using pp = pokesim::internal::unsignedIntType<MechanicConstants::MoveMaxPp::MAX>;
using basePower = pokesim::internal::unsignedIntType<MechanicConstants::MoveBasePower::MAX>;
using power = pokesim::internal::unsignedIntType<MechanicConstants::MovePower::MAX>;
using baseAccuracy = pokesim::internal::unsignedIntType<MechanicConstants::MoveBaseAccuracy::MAX>;
using moveHits = pokesim::internal::unsignedIntType<MechanicConstants::PokemonHpStat::MAX>;

using priority =
  pokesim::internal::signedIntType<MechanicConstants::MovePriority::MAX, MechanicConstants::MovePriority::MIN>;
using fractionalPriority = bool;
}  // namespace pokesim::types