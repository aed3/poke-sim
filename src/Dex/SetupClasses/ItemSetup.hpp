#pragma once

#include <Types/Item.hpp>

#include "Setup.hpp"

namespace pokesim::dex::internal {
struct ItemSetup : DexDataSetup {
  ItemSetup(Dex& pokedex) : DexDataSetup(pokedex) {}

  void setName(Item item);
};
}  // namespace pokesim::dex::internal