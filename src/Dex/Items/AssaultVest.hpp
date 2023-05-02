#pragma once

#include <Dex/Dex.hpp>
#include <Dex/Setup/ItemSetup.hpp>

namespace pokesim::dex::build {
struct AssaultVest {
  static const dex::Item name = dex::ASSAULT_VEST;

  static entt::entity build(Dex& pokedex) {
    internal::ItemSetup item(pokedex);
    item.setName(name);

    return item.entity();
  }
};
}  // namespace pokesim::dex::build