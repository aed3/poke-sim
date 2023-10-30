#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Pokedex/Setup/MoveDexDataSetup.hpp>
#include <Types/Move.hpp>
#include <Types/Type.hpp>
#include <cstdint>

namespace pokesim::dex::build {
struct Thunderbolt {
  static const dex::Move name = dex::THUNDERBOLT;
  static const std::uint8_t accuracy = 100, basePower = 90, basePP = 15;
  static const std::uint8_t secondaryEffectChance = 10;

  static entt::entity build(Pokedex& pokedex) {
    internal::MoveDexDataSetup move(pokedex);
    move.setName(name);
    move.setType(dex::ELECTRIC_TYPE);
    move.setAccuracy(accuracy);
    move.setBasePower(basePower);

    move.setCategoryPhysical();
    move.setBasePP(basePP);

    move.setProperty<pokesim::tags::move::AnySingleTarget>();

    internal::MoveEffectSetup effect(pokedex);
    effect.setChance(secondaryEffectChance);
    effect.setEffectsTarget();
    effect.setProperty<pokesim::tags::status::Paralysis>();

    move.setSecondaryEffect(effect.entity());

    return move.entity();
  }
};
}  // namespace pokesim::dex::build