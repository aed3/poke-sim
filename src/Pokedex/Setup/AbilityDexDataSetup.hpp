#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Registry.hpp>

#include "DexDataSetup.hpp"

namespace pokesim::dex::internal {
struct AbilityDexDataSetup : DexDataSetup {
  AbilityDexDataSetup(types::registry& registry) : DexDataSetup(registry) {}

  void setName(Ability ability);
};
}  // namespace pokesim::dex::internal