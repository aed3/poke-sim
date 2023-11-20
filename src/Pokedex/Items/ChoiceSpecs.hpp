#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/ItemDexDataSetup.hpp>
#include <Types/Enums/Item.hpp>

namespace pokesim::dex::build {
struct ChoiceSpecs {
  static const dex::Item name = dex::Item::CHOICE_SPECS;

  static entt::entity build(Pokedex& pokedex) {
    internal::ItemDexDataSetup item(pokedex);
    item.setName(name);

    return item.entity();
  }
};
}  // namespace pokesim::dex::build