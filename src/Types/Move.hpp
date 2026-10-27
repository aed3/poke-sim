#pragma once

#include <Utilities/NumberToType.hpp>

#include "Constants.hpp"

namespace pokesim::types {
using pp = pokesim::internal::unsignedIntType<Constants::MoveMaxPp::MAX>;
using basePower = pokesim::internal::unsignedIntType<Constants::MoveBasePower::MAX>;
using power = pokesim::internal::unsignedIntType<Constants::MovePower::MAX>;
using baseAccuracy = pokesim::internal::unsignedIntType<Constants::MoveBaseAccuracy::MAX>;
using accuracy = pokesim::internal::unsignedIntType<Constants::MoveAccuracy::MAX>;
using moveHits = pokesim::internal::unsignedIntType<Constants::MoveHits::MAX>;

using priority = pokesim::internal::signedIntType<Constants::MovePriority::MAX, Constants::MovePriority::MIN>;

using moveUses = pokesim::internal::unsignedIntType<Constants::PokemonHpStat::MAX>;
}  // namespace pokesim::types
