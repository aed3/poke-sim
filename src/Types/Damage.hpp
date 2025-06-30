#pragma once

#include "MechanicConstants.hpp"
#include "NumberToType.hpp"

namespace pokesim::types {
using damage = pokesim::internal::unsignedIntType<MechanicConstants::Damage::MAX>;
using damageRollIndex = pokesim::internal::unsignedIntType<MechanicConstants::DamageRollCount::MAX>;
using critBoost = pokesim::internal::unsignedIntType<MechanicConstants::CritBoost::MAX>;
using typeEffectiveness = pokesim::internal::signedIntType<
  MechanicConstants::TypeEffectivenessShift::MAX, MechanicConstants::TypeEffectivenessShift::MIN>;
}  // namespace pokesim::types