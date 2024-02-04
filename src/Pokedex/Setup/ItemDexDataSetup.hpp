#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Types/Enums/Item.hpp>

#include "DexDataSetup.hpp"

namespace pokesim::dex::internal {
struct ItemDexDataSetup : DexDataSetup {
  ItemDexDataSetup(Pokedex* pokedex) : DexDataSetup(pokedex) {}

  void setName(Item item);
};
}  // namespace pokesim::dex::internal