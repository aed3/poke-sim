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
  SpeciesDexDataSetup(Pokedex* pokedex) : DexDataSetup(pokedex) {}

  /*_inline_*/ void setName(Species species);
  /*_inline_*/ void setType(Type type1, Type type2 = Type::NO_TYPE);
  /*_inline_*/ void setBaseStats(
    types::BaseStat hp, types::BaseStat atk, types::BaseStat def, types::BaseStat spa, types::BaseStat spd,
    types::BaseStat spe);
  /*_inline_*/ void setPrimaryAbility(Ability ability);
  /*_inline_*/ void setSecondaryAbility(Ability ability);
  /*_inline_*/ void setHiddenAbility(Ability ability);
};
}  // namespace pokesim::dex::internal