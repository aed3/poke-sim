#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/SpeciesDexDataSetup.hpp>
#include <Types/Species.hpp>
#include <Types/Type.hpp>
#include <cstdint>

namespace pokesim::dex::build {
struct Dragapult {
  static const dex::Species name = dex::DRAGAPULT;
  static const std::uint8_t hp = 88, atk = 120, def = 75, spa = 100, spd = 75, spe = 142;

  static entt::entity build(Pokedex& pokedex) {
    internal::SpeciesDexDataSetup species(pokedex);
    species.setName(name);
    species.setType(dex::DRAGON_TYPE, dex::GHOST_TYPE);
    species.setBaseStats(hp, atk, def, spa, spd, spe);

    return species.entity();
  }
};
}  // namespace pokesim::dex::build