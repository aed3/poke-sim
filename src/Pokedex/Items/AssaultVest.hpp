#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/ItemDexDataSetup.hpp>

namespace pokesim::dex::build {
struct AssaultVest {
  static const dex::Item name = dex::ASSAULT_VEST;

  static entt::entity build(Pokedex& pokedex) {
    internal::ItemDexDataSetup item(pokedex);
    item.setName(name);

    return item.entity();
  }
};
}  // namespace pokesim::dex::build