#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>

#include "DexDataSetup.hpp"

namespace pokesim::dex::internal {
struct SpeciesDexDataSetup : DexDataSetup {
  SpeciesDexDataSetup(Pokedex& pokedex) : DexDataSetup(pokedex) {}

  /*_inline_*/ void setName(Species species);
  /*_inline_*/ void setType(Type type1, Type type2 = NO_TYPE);
  /*_inline_*/ void setBaseStats(
    types::BaseStat hp, types::BaseStat atk, types::BaseStat def, types::BaseStat spa, types::BaseStat spd,
    types::BaseStat spe);
};
}  // namespace pokesim::dex::internal