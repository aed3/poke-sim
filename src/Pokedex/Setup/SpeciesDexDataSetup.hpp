#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>

#include "DexDataSetup.hpp"

namespace pokesim {
class Pokedex;
}

namespace pokesim::dex::internal {
struct SpeciesDexDataSetup : DexDataSetup {
  SpeciesDexDataSetup(types::registry& registry) : DexDataSetup(registry) {}

  void setName(Species species);
  void setType(Type type1, Type type2 = Type::NO_TYPE);
  void setBaseStats(
    types::baseStat hp, types::baseStat atk, types::baseStat def, types::baseStat spa, types::baseStat spd,
    types::baseStat spe);
  void setPrimaryAbility(Ability ability);
  void setSecondaryAbility(Ability ability);
  void setHiddenAbility(Ability ability);
};
}  // namespace pokesim::dex::internal