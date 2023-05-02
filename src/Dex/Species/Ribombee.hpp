#pragma once

#include <Dex/Dex.hpp>
#include <Dex/Setup/SpeciesSetup.hpp>

namespace pokesim::dex::build {
struct Ribombee {
  static const dex::Species name = dex::RIBOMBEE;
  static const std::uint8_t hp = 60, atk = 55, def = 60, spa = 95, spd = 70, spe = 124;

  static entt::entity build(Dex& pokedex) {
    internal::SpeciesSetup species(pokedex);
    species.setName(name);
    species.setType(dex::BUG_TYPE, dex::FAIRY_TYPE);
    species.setBaseStats(hp, atk, def, spa, spd, spe);

    return species.entity();
  }
};
}  // namespace pokesim::dex::build