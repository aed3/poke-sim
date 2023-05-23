#pragma once

#include "Setup.hpp"

namespace pokesim {
struct SideSetup : internal::BattleStateSetup {
  SideSetup(entt::registry& registry) : BattleStateSetup(registry, registry.create()) {}
  SideSetup(entt::registry& registry, entt::entity entity) : BattleStateSetup(registry, entity) {}

  /*_inline_*/ void initBlank();

  /*_inline*/ void setTeam(const std::vector<entt::entity>& team);
  /*_inline*/ void setOpponent(entt::entity entity);
  /*_inline*/ void setBattle(entt::entity entity);

  /*_inline*/ void addTeamMember(entt::entity entity);
};
}  // namespace pokesim