#pragma once

#include <Components/Names/Item.hpp>

#include "Setup.hpp"

namespace pokesim::dex::internal {
struct ItemSetup : DexDataSetup {
  ItemSetup(Dex& pokedex) : DexDataSetup(pokedex) {}

  void setName(Item item);
};
}  // namespace pokesim::dex::internal