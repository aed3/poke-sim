#pragma once

#include "StateSetupBase.hpp"

namespace pokesim {
struct SideStateSetup : internal::StateSetupBase {
  SideStateSetup(entt::registry& registry) : StateSetupBase(registry, registry.create()) {}
  SideStateSetup(entt::registry& registry, entt::entity entity) : StateSetupBase(registry, entity) {}

  /*_inline_*/ void initBlank();

  /*_inline_*/ void setTeam(const std::vector<entt::entity>& team);
  /*_inline_*/ void setOpponent(entt::entity entity);
  /*_inline_*/ void setBattle(entt::entity entity);

  /*_inline_*/ void addTeamMember(entt::entity entity);
};
}  // namespace pokesim