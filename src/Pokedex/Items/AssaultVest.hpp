#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/ItemDexDataSetup.hpp>
#include <Types/Enums/Item.hpp>

namespace pokesim::dex::build {
struct AssaultVest {
  static const dex::Item name = dex::Item::ASSAULT_VEST;

  static entt::entity build(Pokedex& pokedex) {
    internal::ItemDexDataSetup item(pokedex);
    item.setName(name);

    return item.entity();
  }
};
}  // namespace pokesim::dex::build