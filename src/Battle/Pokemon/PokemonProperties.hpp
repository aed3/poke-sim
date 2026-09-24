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
class Pokedex;
struct CurrentActionTarget;
struct TypeName;

constexpr types::typeEffectiveness getAttackEffectiveness(
  SpeciesTypes speciesTypes, dex::Type attackingType, const TypeChart& typeChart) {
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

constexpr bool isSpeciesTypeImmune(SpeciesTypes speciesTypes, dex::Type attackingType, const TypeChart& typeChart) {
  for (dex::Type defendingType : speciesTypes.val) {
    if (typeChart.effectiveness(attackingType, defendingType) == TypeEffectiveness::IMMUNE) {
      return true;
    }
  }
  return false;
}

namespace internal {
bool doesMoveMakeContact(types::registry& registry, types::entity move, types::entity source);
bool isGrounded(types::registry& registry, types::entity entity);
bool isTargetImmune(types::registry& registry, CurrentActionTarget target, TypeName typeName, const Pokedex& pokedex);
}  // namespace internal
}  // namespace pokesim
