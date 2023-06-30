#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/DexDataItemSetup.hpp>

namespace pokesim::dex::build {
struct BrightPowder {
  static const dex::Item name = dex::BRIGHT_POWDER;

  static entt::entity build(Pokedex& pokedex) {
    internal::ItemSetup item(pokedex);
    item.setName(name);

    return item.entity();
  }
};
}  // namespace pokesim::dex::build