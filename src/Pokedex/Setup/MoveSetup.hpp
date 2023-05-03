#pragma once

#include <Components/Boosts.hpp>
#include <Components/Tags/Move.hpp>
#include <Types/Move.hpp>

#include "Setup.hpp"

namespace pokesim::dex::internal {
struct MoveSetup : DexDataSetup {
  MoveSetup(Pokedex& pokedex) : DexDataSetup(pokedex) {}

  /*_inline_*/ void setName(Move move);
  /*_inline_*/ void setType(Type type);
  /*_inline_*/ void setAccuracy(std::uint8_t accuracy);
  /*_inline_*/ void setBasePower(std::uint8_t basePower);

  /*_inline_*/ void setCategoryPhysical();
  /*_inline_*/ void setCategorySpecial();
  /*_inline_*/ void setCategoryStatus();

  /*_inline_*/ void setBasePP(std::uint8_t pp);
  /*_inline_*/ void setPriority(std::uint8_t priority);
  /*_inline_*/ void setMultiHit(std::uint8_t minHits, std::uint8_t maxHits);

  /*_inline_*/ void setPrimaryEffect(entt::entity entity);
  /*_inline_*/ void setSecondaryEffect(entt::entity entity);
};

struct MoveEffectSetup : DexDataSetup {
  MoveEffectSetup(Pokedex& pokedex) : DexDataSetup(pokedex) {}
  entt::entity entity() const { return handle; }

  /*_inline_*/ void setChance(std::uint8_t chance);
  /*_inline_*/ void setEffectsSelf();
  /*_inline_*/ void setEffectsTarget();

  template <typename BoostType>
  /*_inline_*/ void setBoost(std::int8_t boost) {
    static_assert(
      std::is_same<AtkBoost, BoostType>() || std::is_same<DefBoost, BoostType>() ||
      std::is_same<SpaBoost, BoostType>() || std::is_same<SpdBoost, BoostType>() ||
      std::is_same<SpeBoost, BoostType>());
    handle.emplace<SpdBoost>(boost);
  }
};
}  // namespace pokesim::dex::internal