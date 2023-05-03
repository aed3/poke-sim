#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/SpeciesSetup.hpp>

namespace pokesim::dex::build {
struct Gardevoir {
  static const dex::Species name = dex::GARDEVOIR;
  static const std::uint8_t hp = 68, atk = 65, def = 65, spa = 125, spd = 115, spe = 80;

  static entt::entity build(Pokedex& pokedex) {
    internal::SpeciesSetup species(pokedex);
    species.setName(name);
    species.setType(dex::PSYCHIC_TYPE, dex::FAIRY_TYPE);
    species.setBaseStats(hp, atk, def, spa, spd, spe);

    return species.entity();
  }
};
}  // namespace pokesim::dex::build