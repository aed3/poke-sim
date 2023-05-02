#pragma once

#include <Types/Species.hpp>
#include <Types/Type.hpp>

#include "Setup.hpp"

namespace pokesim::dex::internal {
struct SpeciesSetup : DexDataSetup {
  SpeciesSetup(Dex& pokedex) : DexDataSetup(pokedex) {}

  /*__inline__*/ void setName(Species species);
  /*__inline__*/ void setType(Type type1, Type type2 = NO_TYPE);
  /*__inline__*/ void setBaseStats(
    std::uint8_t hp, std::uint8_t atk, std::uint8_t def, std::uint8_t spa, std::uint8_t spd, std::uint8_t spe);
};
}  // namespace pokesim::dex::internal