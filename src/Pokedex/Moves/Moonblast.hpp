#pragma once

#include <Components/Boosts.hpp>
#include <Components/Tags/MoveTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/MoveDexDataSetup.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Types/Stats.hpp>

namespace pokesim::dex::build {
struct Moonblast {
  static const dex::Move name = dex::MOONBLAST;
  static const types::BaseAccuracy accuracy = 100;
  static const types::BasePower basePower = 95;
  static const types::Pp basePp = 15;
  static const types::BaseEffectChance secondaryEffectChance = 30;
  static const types::Boost spaBoost = -1;

  static entt::entity build(Pokedex& pokedex) {
    internal::MoveDexDataSetup move(pokedex);
    move.setName(name);
    move.setType(dex::FAIRY_TYPE);
    move.setAccuracy(accuracy);
    move.setBasePower(basePower);

    move.setCategorySpecial();
    move.setBasePP(basePp);

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