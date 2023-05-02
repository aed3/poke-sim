#pragma once

#include <Components/Tags/Status.hpp>
#include <Dex/Dex.hpp>
#include <Dex/Setup/MoveSetup.hpp>

namespace pokesim::dex::build {
struct Thunderbolt {
  static const dex::Move name = dex::THUNDERBOLT;
  static const std::uint8_t accuracy = 100, basePower = 90, basePP = 15;
  static const std::uint8_t secondaryEffectChance = 10;

  static entt::entity build(Dex& pokedex) {
    internal::MoveSetup move(pokedex);
    move.setName(name);
    move.setType(dex::ELECTRIC_TYPE);
    move.setAccuracy(accuracy);
    move.setBasePower(basePower);

    move.setCategoryPhysical();
    move.setBasePP(basePP);

    move.setProperty<pokesim::move::AnySingleTarget>();

    internal::MoveEffectSetup effect(pokedex);
    effect.setChance(secondaryEffectChance);
    effect.setEffectsTarget();
    effect.setProperty<pokesim::status::Paralysis>();

    move.setSecondaryEffect(effect.entity());

    return move.entity();
  }
};
}  // namespace pokesim::dex::build