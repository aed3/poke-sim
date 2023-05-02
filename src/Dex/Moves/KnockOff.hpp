#pragma once

#include <Dex/Dex.hpp>
#include <Dex/Setup/MoveSetup.hpp>

namespace pokesim::dex::build {
struct KnockOff {
  static const dex::Move name = dex::KNOCK_OFF;
  static const std::uint8_t accuracy = 100, basePower = 65, basePP = 20;

  static entt::entity build(Dex& pokedex) {
    internal::MoveSetup move(pokedex);
    move.setName(name);
    move.setType(dex::DARK_TYPE);
    move.setAccuracy(accuracy);
    move.setBasePower(basePower);

    move.setCategoryPhysical();
    move.setBasePP(basePP);

    move.setProperty<pokesim::move::AnySingleTarget>();
    move.setProperty<pokesim::move::Contact>();

    return move.entity();
  }
};
}  // namespace pokesim::dex::build