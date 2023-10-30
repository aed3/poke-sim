#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/ItemDexDataSetup.hpp>
#include <Types/Item.hpp>

namespace pokesim::dex::build {
struct ChoiceScarf {
  static const dex::Item name = dex::CHOICE_SCARF;

  static entt::entity build(Pokedex& pokedex) {
    internal::ItemDexDataSetup item(pokedex);
    item.setName(name);

    return item.entity();
  }
};
}  // namespace pokesim::dex::build