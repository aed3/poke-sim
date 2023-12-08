#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/MoveDexDataSetup.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>

namespace pokesim::dex::build {
struct WillOWisp {
  static const dex::Move name = dex::Move::WILL_O_WISP;
  static const types::BaseAccuracy accuracy = 85;
  static const types::Pp basePp = 15;

  static entt::entity build(Pokedex& pokedex) {
    internal::MoveDexDataSetup move(pokedex);
    move.setName(name);
    move.setType(dex::Type::FIRE_TYPE);
    move.setAccuracy(accuracy);

    move.setCategoryStatus();
    move.setBasePP(basePp);

    move.setProperty<pokesim::tags::move::AnySingleTarget>();

    internal::MoveEffectSetup effect(pokedex);
    effect.setEffectsTarget();
    effect.setProperty<pokesim::tags::status::Burn>();

    move.setPrimaryEffect(effect.entity());

    return move.entity();
  }
};
}  // namespace pokesim::dex::build