#pragma once

#include <Dex/Dex.hpp>
#include <Dex/Setup/ItemSetup.hpp>

namespace pokesim::dex::build {
struct ChoiceScarf {
  static const dex::Item name = dex::CHOICE_SCARF;

  static entt::entity build(Dex& pokedex) {
    internal::ItemSetup species(pokedex);
    species.setName(name);

    return species.entity();
  }
};
}  // namespace pokesim::dex::build