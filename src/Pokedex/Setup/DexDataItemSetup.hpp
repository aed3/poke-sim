#pragma once

#include <Types/Item.hpp>

#include "DexDataSetup.hpp"

namespace pokesim::dex::internal {
struct ItemSetup : DexDataSetup {
  ItemSetup(Pokedex& pokedex) : DexDataSetup(pokedex) {}

  /*_inline_*/ void setName(Item item);
};
}  // namespace pokesim::dex::internal