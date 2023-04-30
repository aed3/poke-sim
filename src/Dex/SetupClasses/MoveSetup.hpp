#pragma once

#include <Components/Names/Move.hpp>

#include "Setup.hpp"

namespace pokesim::dex::internal {
struct MoveSetup : DexDataSetup {
  MoveSetup(Dex& pokedex) : DexDataSetup(pokedex) {}

  void setName(Move move);
  void setType(Type type);
  void setAccuracy(std::uint8_t accuracy);
  void setBasePower(std::uint8_t basePower);

  void setCategoryPhysical();
  void setCategorySpecial();
  void setCategoryStatus();

  void setBasePP(std::uint8_t pp);
  void setPriority(std::uint8_t priority);
  void setMultiHit(std::uint8_t minHits, std::uint8_t maxHits);

  void setPrimaryEffect(entt::entity entity);
  void setSecondaryEffect(entt::entity entity);
};

struct MoveEffectSetup : DexDataSetup {
  MoveEffectSetup(Dex& pokedex) : DexDataSetup(pokedex) {}
  entt::entity entity() const { return handle; }

  void setChance(std::uint8_t chance);
  void setEffectsSelf();
  void setEffectsTarget();

  template <typename BoostType>
  void setBoost(std::int8_t boost);
};
}  // namespace pokesim::dex::internal