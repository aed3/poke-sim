#pragma once

#include <Components/EntityHolders/Side.hpp>

#include "Setup.hpp"

namespace pokesim {
struct BattleSetup : internal::BattleStateSetup {
  BattleSetup(entt::registry& registry) : BattleStateSetup(registry, registry.create()) {}
  BattleSetup(entt::registry& registry, entt::entity entity) : BattleStateSetup(registry, entity) {}

  /*_inline_*/ void initBlank();

  /*_inline*/ void setID(std::uint16_t id);
  /*_inline*/ void setSide(Side::PlayerSideID sideID, entt::entity sideEntity);
  /*_inline*/ void setRNGSeed(std::uint32_t seed);
  /*_inline*/ void setActionQueue(const std::vector<entt::entity>& queue);
  /*_inline*/ void setTurn(std::uint16_t turn);
  /*_inline*/ void setActiveMove(entt::entity activeMove);
  /*_inline*/ void setActivePokemon(entt::entity activePokemon);
  /*_inline*/ void setActiveTarget(entt::entity activeTarget);
  /*_inline*/ void setActiveSource(entt::entity activeSource);
  /*_inline*/ void setProbability(float probability);
};
}  // namespace pokesim