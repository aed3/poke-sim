#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Registry.hpp>

#include "DexDataSetup.hpp"

namespace pokesim::internal::dex {
struct AbilityDexDataSetup : DexDataSetup {
  AbilityDexDataSetup(types::registry& registry) : DexDataSetup(registry) {}

  void setName(pokesim::dex::Ability ability);
};
}  // namespace pokesim::internal::dex
