#pragma once

#include <Components/Boosts.hpp>
#include <Components/Tags/Move.hpp>
#include <Types/Move.hpp>

#include "Setup.hpp"

namespace pokesim::dex::internal {
struct MoveSetup : DexDataSetup {
  MoveSetup(Dex& pokedex) : DexDataSetup(pokedex) {}

  /*__inline__*/ void setName(Move move);
  /*__inline__*/ void setType(Type type);
  /*__inline__*/ void setAccuracy(std::uint8_t accuracy);
  /*__inline__*/ void setBasePower(std::uint8_t basePower);

  /*__inline__*/ void setCategoryPhysical();
  /*__inline__*/ void setCategorySpecial();
  /*__inline__*/ void setCategoryStatus();

  /*__inline__*/ void setBasePP(std::uint8_t pp);
  /*__inline__*/ void setPriority(std::uint8_t priority);
  /*__inline__*/ void setMultiHit(std::uint8_t minHits, std::uint8_t maxHits);

  /*__inline__*/ void setPrimaryEffect(entt::entity entity);
  /*__inline__*/ void setSecondaryEffect(entt::entity entity);
};

struct MoveEffectSetup : DexDataSetup {
  MoveEffectSetup(Dex& pokedex) : DexDataSetup(pokedex) {}
  entt::entity entity() const { return handle; }

  /*__inline__*/ void setChance(std::uint8_t chance);
  /*__inline__*/ void setEffectsSelf();
  /*__inline__*/ void setEffectsTarget();

  template <typename BoostType>
  /*__inline__*/ void setBoost(std::int8_t boost) {
    static_assert(
      std::is_same<AtkBoost, BoostType>() || std::is_same<DefBoost, BoostType>() ||
      std::is_same<SpaBoost, BoostType>() || std::is_same<SpdBoost, BoostType>() ||
      std::is_same<SpeBoost, BoostType>());
    handle.emplace<SpdBoost>(boost);
  }
};
}  // namespace pokesim::dex::internal