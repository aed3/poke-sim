#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Registry.hpp>

#include "DexDataSetup.hpp"

namespace pokesim::internal::dex {
struct ItemDexDataSetup : DexDataSetup {
  ItemDexDataSetup(types::registry& registry) : DexDataSetup(registry) {}

  void setName(pokesim::dex::Item item);
};
}  // namespace pokesim::internal::dex
