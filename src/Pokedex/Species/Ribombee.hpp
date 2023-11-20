#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/SpeciesDexDataSetup.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>

namespace pokesim::dex::build {
struct Ribombee {
  static const dex::Species name = dex::Species::RIBOMBEE;
  static const types::BaseStat hp = 60, atk = 55, def = 60, spa = 95, spd = 70, spe = 124;

  static entt::entity build(Pokedex& pokedex) {
    internal::SpeciesDexDataSetup species(pokedex);
    species.setName(name);
    species.setType(dex::Type::BUG_TYPE, dex::Type::FAIRY_TYPE);
    species.setBaseStats(hp, atk, def, spa, spd, spe);

    return species.entity();
  }
};
}  // namespace pokesim::dex::build