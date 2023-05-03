#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/ItemSetup.hpp>

namespace pokesim::dex::build {
struct ChoiceSpecs {
  static const dex::Item name = dex::CHOICE_SPECS;

  static entt::entity build(Pokedex& pokedex) {
    internal::ItemSetup item(pokedex);
    item.setName(name);

    return item.entity();
  }
};
}  // namespace pokesim::dex::build