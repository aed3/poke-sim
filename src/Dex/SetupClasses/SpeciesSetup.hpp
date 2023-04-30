#pragma once

#include <Components/Names/Species.hpp>
#include <Components/Names/Type.hpp>

#include "Setup.hpp"

namespace pokesim::dex::internal {
struct SpeciesSetup : DexDataSetup {
  SpeciesSetup(Dex& pokedex) : DexDataSetup(pokedex) {}

  void setName(Species species);
  void setType(Type type1, Type type2 = NO_TYPE);
  void setBaseStats(
    std::uint8_t hp, std::uint8_t atk, std::uint8_t def, std::uint8_t spa, std::uint8_t spd, std::uint8_t spe);
};
}  // namespace pokesim::dex::internal