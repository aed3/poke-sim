#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/MoveDexDataSetup.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>

namespace pokesim::dex::build {
struct KnockOff {
  static const dex::Move name = dex::Move::KNOCK_OFF;
  static const types::BaseAccuracy accuracy = 100;
  static const types::BasePower basePower = 65;
  static const types::Pp basePp = 20;

  static entt::entity build(Pokedex& pokedex) {
    internal::MoveDexDataSetup move(pokedex);
    move.setName(name);
    move.setType(dex::Type::DARK_TYPE);
    move.setAccuracy(accuracy);
    move.setBasePower(basePower);

    move.setCategoryPhysical();
    move.setBasePP(basePp);

    move.setProperty<pokesim::tags::move::AnySingleTarget>();
    move.setProperty<pokesim::tags::move::Contact>();

    return move.entity();
  }
};
}  // namespace pokesim::dex::build