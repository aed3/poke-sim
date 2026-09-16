#pragma once

#include <Utilities/NumberToType.hpp>

#include "Constants.hpp"

namespace pokesim::types {
using damage = pokesim::internal::unsignedIntType<Constants::Damage::MAX>;
using damageRollIndex = pokesim::internal::unsignedIntType<Constants::DamageRollCount::MAX>;
using critStage = pokesim::internal::unsignedIntType<Constants::CritStage::MAX>;
using typeEffectiveness =
  pokesim::internal::signedIntType<Constants::TypeEffectivenessShift::MAX, Constants::TypeEffectivenessShift::MIN>;
}  // namespace pokesim::types
