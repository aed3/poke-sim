#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Stats.hpp>
#include <limits>

namespace pokesim {
constexpr types::boost getAttackEffectiveness(const SpeciesTypes& speciesTypes, dex::Type attackingType) {
  types::boost modifier = 0;
  for (dex::Type defendingType : speciesTypes.val) {
    switch (MechanicConstants::TYPE_CHART.effectiveness(attackingType, defendingType)) {
      case TypeEffectiveness::IMMUNE: {
        return -std::numeric_limits<types::boost>::digits;
      }
      case TypeEffectiveness::NEUTRAL: {
        break;
      }
      case TypeEffectiveness::NOT_VERY_EFFECTIVE: {
        modifier--;
        break;
      }
      case TypeEffectiveness::SUPER_EFFECTIVE: {
        modifier++;
      }
    }
  }

  return modifier;
}
}  // namespace pokesim