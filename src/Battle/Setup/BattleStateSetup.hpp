#pragma once

#include <Components/EntityHolders/Side.hpp>

#include "StateSetupBase.hpp"

namespace pokesim {
struct BattleStateSetup : internal::StateSetupBase {
  BattleStateSetup(entt::registry& registry) : StateSetupBase(registry, registry.create()) {}
  BattleStateSetup(entt::registry& registry, entt::entity entity) : StateSetupBase(registry, entity) {}

  /*_inline_*/ void initBlank();

  /*_inline_*/ void setAutoID();
  /*_inline_*/ void setID(std::uint16_t id);
  /*_inline_*/ void setSide(Side::PlayerSideID sideID, entt::entity sideEntity);
  /*_inline_*/ void setRNGSeed(std::uint32_t seed);
  /*_inline_*/ void setActionQueue(const std::vector<entt::entity>& queue);
  /*_inline_*/ void setTurn(std::uint16_t turn);
  /*_inline_*/ void setActiveMove(entt::entity activeMove);
  /*_inline_*/ void setActivePokemon(entt::entity activePokemon);
  /*_inline_*/ void setActiveTarget(entt::entity activeTarget);
  /*_inline_*/ void setActiveUser(entt::entity activeSource);
  /*_inline_*/ void setProbability(float probability);
};
}  // namespace pokesim