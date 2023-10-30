#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/MoveDexDataSetup.hpp>
#include <Types/Move.hpp>
#include <Types/Type.hpp>
#include <cstdint>

namespace pokesim::dex::build {
struct WillOWisp {
  static const dex::Move name = dex::WILL_O_WISP;
  static const std::uint8_t accuracy = 85, basePP = 15;

  static entt::entity build(Pokedex& pokedex) {
    internal::MoveDexDataSetup move(pokedex);
    move.setName(name);
    move.setType(dex::FIRE_TYPE);
    move.setAccuracy(accuracy);

    move.setCategoryStatus();
    move.setBasePP(basePP);

    move.setProperty<pokesim::tags::move::AnySingleTarget>();

    internal::MoveEffectSetup effect(pokedex);
    effect.setEffectsTarget();
    effect.setProperty<pokesim::tags::status::Burn>();

    move.setPrimaryEffect(effect.entity());

    return move.entity();
  }
};
}  // namespace pokesim::dex::build