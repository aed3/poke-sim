#pragma once

#include "MechanicConstants.hpp"
#include "NumberToType.hpp"

namespace pokesim::types {
using pp = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::MOVE_MAX_PP>;
using basePower = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::MOVE_BASE_POWER>;
using baseAccuracy = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::MOVE_BASE_ACCURACY>;
using moveHits = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::MOVE_HITS>;
using baseEffectChance = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::MOVE_BASE_EFFECT_CHANCE>;

using priority = pokesim::internal::signedIntType<
  MechanicConstants::MaxValues::MOVE_PRIORITY, MechanicConstants::MinValues::MOVE_PRIORITY>;
using fractionalPriority = bool;
}  // namespace pokesim::types