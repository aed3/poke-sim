#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/MoveDexDataSetup.hpp>
#include <Types/Move.hpp>
#include <Types/Type.hpp>
#include <cstdint>

namespace pokesim::dex::build {
struct KnockOff {
  static const dex::Move name = dex::KNOCK_OFF;
  static const std::uint8_t accuracy = 100, basePower = 65, basePP = 20;

  static entt::entity build(Pokedex& pokedex) {
    internal::MoveDexDataSetup move(pokedex);
    move.setName(name);
    move.setType(dex::DARK_TYPE);
    move.setAccuracy(accuracy);
    move.setBasePower(basePower);

    move.setCategoryPhysical();
    move.setBasePP(basePP);

    move.setProperty<pokesim::tags::move::AnySingleTarget>();
    move.setProperty<pokesim::tags::move::Contact>();

    return move.entity();
  }
};
}  // namespace pokesim::dex::build