#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>

#include "DexDataSetup.hpp"

namespace pokesim {
class Pokedex;
}

namespace pokesim::internal::dex {
struct SpeciesDexDataSetup : DexDataSetup {
  SpeciesDexDataSetup(types::registry& registry) : DexDataSetup(registry) {}

  void setName(pokesim::dex::Species species);
  void setType(pokesim::dex::Type type1, pokesim::dex::Type type2 = pokesim::dex::Type::NO_TYPE);
  void setBaseStats(
    types::baseStat hp, types::baseStat atk, types::baseStat def, types::baseStat spa, types::baseStat spd,
    types::baseStat spe);
  void setPrimaryAbility(pokesim::dex::Ability ability);
  void setSecondaryAbility(pokesim::dex::Ability ability);
  void setHiddenAbility(pokesim::dex::Ability ability);
};
}  // namespace pokesim::internal::dex
