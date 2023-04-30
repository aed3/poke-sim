#pragma once

#include <Components/Names/Move.hpp>
#include <Dex/Dex.hpp>

namespace pokesim::dex::internal {
class MoveSetup {
  entt::handle handle;

 public:
  MoveSetup(Dex& pokedex) : handle(pokedex.createEntry()) {}

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

  template <typename Tag>
  void setProperty();
};

class MoveEffectSetup {
  entt::handle handle;

 public:
  MoveEffectSetup(Dex& pokedex) : handle(pokedex.createEntry()) {}
  entt::entity entity() const { return handle; }

  void setChance(std::uint8_t chance);
  void setEffectsSelf();
  void setEffectsTarget();

  template <typename BoostType>
  void setBoost(std::int8_t boost);

  template <typename Tag>
  void setProperty();
};
}  // namespace pokesim::dex::internal