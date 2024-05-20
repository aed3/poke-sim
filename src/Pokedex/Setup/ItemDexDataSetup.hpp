#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Types/Enums/Item.hpp>

#include "DexDataSetup.hpp"

namespace pokesim::dex::internal {
struct ItemDexDataSetup : DexDataSetup {
  ItemDexDataSetup(types::registry& registry) : DexDataSetup(registry) {}

  void setName(Item item);
};
}  // namespace pokesim::dex::internal