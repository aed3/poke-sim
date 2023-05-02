#pragma once

#include <Components/Boosts.hpp>
#include <Dex/Dex.hpp>
#include <Dex/Setup/MoveSetup.hpp>

namespace pokesim::dex::build {
struct QuiverDance {
  static const dex::Move name = dex::QUIVER_DANCE;
  static const std::uint8_t basePP = 20;
  static const std::int8_t spaBoost = 1, spdBoost = 1, speBoost = 1;

  static entt::entity build(Dex& pokedex) {
    internal::MoveSetup move(pokedex);
    move.setName(name);
    move.setType(dex::BUG_TYPE);

    move.setCategoryStatus();
    move.setBasePP(basePP);

    move.setProperty<pokesim::move::Self>();

    internal::MoveEffectSetup effect(pokedex);
    effect.setEffectsSelf();
    effect.setBoost<SpaBoost>(spaBoost);
    effect.setBoost<SpdBoost>(spdBoost);
    effect.setBoost<SpeBoost>(speBoost);

    move.setPrimaryEffect(effect.entity());

    return move.entity();
  }
};
}  // namespace pokesim::dex::build