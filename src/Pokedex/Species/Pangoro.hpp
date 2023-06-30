#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/DexDataSpeciesSetup.hpp>

namespace pokesim::dex::build {
struct Pangoro {
  static const dex::Species name = dex::PANGORO;
  static const std::uint8_t hp = 95, atk = 124, def = 78, spa = 69, spd = 71, spe = 58;

  static entt::entity build(Pokedex& pokedex) {
    internal::SpeciesSetup species(pokedex);
    species.setName(name);
    species.setType(dex::FIGHTING_TYPE, dex::DARK_TYPE);
    species.setBaseStats(hp, atk, def, spa, spd, spe);

    return species.entity();
  }
};
}  // namespace pokesim::dex::build