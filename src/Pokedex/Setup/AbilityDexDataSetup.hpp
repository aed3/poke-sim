#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Types/Enums/Ability.hpp>

#include "DexDataSetup.hpp"

namespace pokesim::dex::internal {
struct AbilityDexDataSetup : DexDataSetup {
  AbilityDexDataSetup(Pokedex* pokedex) : DexDataSetup(pokedex) {}

  void setName(Ability ability);
};
}  // namespace pokesim::dex::internal