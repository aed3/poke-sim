#pragma once

#include "MechanicConstants.hpp"
#include "NumberToType.hpp"

namespace pokesim::types {
using damage = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::DAMAGE>;
using damageRollIndex = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::DAMAGE_ROLL_COUNT>;
using useUntilKoChance = float;
using typeEffectiveness = pokesim::internal::signedIntType<
  MechanicConstants::MaxValues::TYPE_EFFECTIVENESS_SHIFT, MechanicConstants::MinValues::TYPE_EFFECTIVENESS_SHIFT>;
}  // namespace pokesim::types