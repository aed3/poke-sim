#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/SpeciesDexDataSetup.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>

namespace pokesim::dex::build {
struct Empoleon {
  static const dex::Species name = dex::EMPOLEON;
  static const types::BaseStat hp = 84, atk = 86, def = 88, spa = 111, spd = 101, spe = 60;

  static entt::entity build(Pokedex& pokedex) {
    internal::SpeciesDexDataSetup species(pokedex);
    species.setName(name);
    species.setType(dex::WATER_TYPE, dex::STEEL_TYPE);
    species.setBaseStats(hp, atk, def, spa, spd, spe);

    return species.entity();
  }
};
}  // namespace pokesim::dex::build