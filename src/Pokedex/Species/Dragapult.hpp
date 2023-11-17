#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/SpeciesDexDataSetup.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>

namespace pokesim::dex::build {
struct Dragapult {
  static const dex::Species name = dex::DRAGAPULT;
  static const types::BaseStat hp = 88, atk = 120, def = 75, spa = 100, spd = 75, spe = 142;

  static entt::entity build(Pokedex& pokedex) {
    internal::SpeciesDexDataSetup species(pokedex);
    species.setName(name);
    species.setType(dex::DRAGON_TYPE, dex::GHOST_TYPE);
    species.setBaseStats(hp, atk, def, spa, spd, spe);

    return species.entity();
  }
};
}  // namespace pokesim::dex::build