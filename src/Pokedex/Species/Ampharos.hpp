#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/SpeciesDexDataSetup.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>

namespace pokesim::dex::build {
struct Ampharos {
  static const dex::Species name = dex::AMPHAROS;
  static const types::BaseStat hp = 90, atk = 75, def = 85, spa = 115, spd = 90, spe = 55;

  static entt::entity build(Pokedex& pokedex) {
    internal::SpeciesDexDataSetup species(pokedex);
    species.setName(name);
    species.setType(dex::ELECTRIC_TYPE);
    species.setBaseStats(hp, atk, def, spa, spd, spe);

    return species.entity();
  }
};
}  // namespace pokesim::dex::build