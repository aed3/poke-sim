#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Types/Species.hpp>
#include <Types/Type.hpp>
#include <cstdint>

#include "DexDataSetup.hpp"

namespace pokesim::dex::internal {
struct SpeciesDexDataSetup : DexDataSetup {
  SpeciesDexDataSetup(Pokedex& pokedex) : DexDataSetup(pokedex) {}

  /*_inline_*/ void setName(Species species);
  /*_inline_*/ void setType(Type type1, Type type2 = NO_TYPE);
  /*_inline_*/ void setBaseStats(
    std::uint8_t hp, std::uint8_t atk, std::uint8_t def, std::uint8_t spa, std::uint8_t spd, std::uint8_t spe);
};
}  // namespace pokesim::dex::internal