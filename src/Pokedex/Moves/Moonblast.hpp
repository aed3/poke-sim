#pragma once

#include <Components/Boosts.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/MoveDexDataSetup.hpp>

namespace pokesim::dex::build {
struct Moonblast {
  static const dex::Move name = dex::MOONBLAST;
  static const std::uint8_t accuracy = 100, basePower = 95, basePP = 15;
  static const std::uint8_t secondaryEffectChance = 30;
  static const std::int8_t spaBoost = -1;

  static entt::entity build(Pokedex& pokedex) {
    internal::MoveDexDataSetup move(pokedex);
    move.setName(name);
    move.setType(dex::FAIRY_TYPE);
    move.setAccuracy(accuracy);
    move.setBasePower(basePower);

    move.setCategorySpecial();
    move.setBasePP(basePP);

    move.setProperty<pokesim::tags::move::AnySingleTarget>();

    internal::MoveEffectSetup effect(pokedex);
    effect.setChance(secondaryEffectChance);
    effect.setEffectsTarget();
    effect.setBoost<SpaBoost>(spaBoost);

    move.setSecondaryEffect(effect.entity());

    return move.entity();
  }
};
}  // namespace pokesim::dex::build