#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/MoveSetup.hpp>

namespace pokesim::dex::build {
struct FuryAttack {
  static const dex::Move name = dex::FURY_ATTACK;
  static const std::uint8_t accuracy = 85, basePower = 15, basePP = 20, minHits = 2, maxHits = 5;

  static entt::entity build(Pokedex& pokedex) {
    internal::MoveSetup move(pokedex);
    move.setName(name);
    move.setType(dex::NORMAL_TYPE);
    move.setAccuracy(accuracy);
    move.setBasePower(basePower);

    move.setCategoryPhysical();
    move.setBasePP(basePP);
    move.setMultiHit(minHits, maxHits);

    move.setProperty<pokesim::move::AnySingleTarget>();
    move.setProperty<pokesim::move::Contact>();

    return move.entity();
  }
};
}  // namespace pokesim::dex::build