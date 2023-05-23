#pragma once

#include "Setup.hpp"

namespace pokesim {
struct SideSetup : internal::BattleStateSetup {
  SideSetup(entt::registry& registry) : BattleStateSetup(registry, registry.create()) {}
  SideSetup(entt::registry& registry, entt::entity entity) : BattleStateSetup(registry, entity) {}

  /*_inline_*/ void initBlank();

  /*_inline_*/ void setTeam(const std::vector<entt::entity>& team);
  /*_inline_*/ void setOpponent(entt::entity entity);
  /*_inline_*/ void setBattle(entt::entity entity);

  /*_inline_*/ void addTeamMember(entt::entity entity);
};
}  // namespace pokesim