#pragma once

#include <Components/Tags/Status.hpp>
#include <Dex/Dex.hpp>
#include <Dex/Setup/MoveSetup.hpp>

namespace pokesim::dex::build {
struct WillOWisp {
  static const dex::Move name = dex::WILL_O_WISP;
  static const std::uint8_t accuracy = 85, basePP = 15;

  static entt::entity build(Dex& pokedex) {
    internal::MoveSetup move(pokedex);
    move.setName(name);
    move.setType(dex::FIRE_TYPE);
    move.setAccuracy(accuracy);

    move.setCategoryStatus();
    move.setBasePP(basePP);

    move.setProperty<pokesim::move::AnySingleTarget>();

    internal::MoveEffectSetup effect(pokedex);
    effect.setEffectsTarget();
    effect.setProperty<pokesim::status::Burn>();

    move.setPrimaryEffect(effect.entity());

    return move.entity();
  }
};
}  // namespace pokesim::dex::build