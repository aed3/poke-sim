#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Config/Require.hpp>
#include <Types/Damage.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Stats.hpp>
#include <limits>

namespace pokesim {
constexpr types::typeEffectiveness getAttackEffectiveness(const SpeciesTypes& speciesTypes, dex::Type attackingType) {
  types::typeEffectiveness modifier = 0;
  for (dex::Type defendingType : speciesTypes.val) {
    switch (MechanicConstants::TYPE_CHART.effectiveness(attackingType, defendingType)) {
      case TypeEffectiveness::IMMUNE: {
        return -std::numeric_limits<types::typeEffectiveness>::digits;
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

  POKESIM_REQUIRE(
    modifier <= speciesTypes.size() && modifier >= -speciesTypes.size(),
    "Modifier cannot exceed the number of types.");
  return modifier;
}
}  // namespace pokesim