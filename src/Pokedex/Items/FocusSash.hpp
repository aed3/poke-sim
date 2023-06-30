#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/DexDataItemSetup.hpp>

namespace pokesim::dex::build {
struct FocusSash {
  static const dex::Item name = dex::FOCUS_SASH;

  static entt::entity build(Pokedex& pokedex) {
    internal::ItemSetup item(pokedex);
    item.setName(name);

    return item.entity();
  }
};
}  // namespace pokesim::dex::build