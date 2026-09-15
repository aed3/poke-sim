#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Config/Require.hpp>
#include <Pokedex/TypeChart.hpp>
#include <Types/Damage.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/TypeEffectiveness.hpp>
#include <Types/Registry.hpp>
#include <Types/Stats.hpp>
#include <limits>

namespace pokesim {
constexpr types::typeEffectiveness getAttackEffectiveness(
  const SpeciesTypes& speciesTypes, dex::Type attackingType, const TypeChart& typeChart) {
  types::typeEffectiveness modifier = 0;
  for (dex::Type defendingType : speciesTypes.val) {
    switch (typeChart.effectiveness(attackingType, defendingType)) {
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

namespace internal {
bool doesMoveMakeContact(types::registry& registry, types::entity move, types::entity source);
}
}  // namespace pokesim
