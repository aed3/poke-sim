#pragma once

#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <vector>

#include "StateSetupBase.hpp"

namespace pokesim {
struct PokemonStateSetup;

// Tool to set properties of a player's side state to an entity.
struct SideStateSetup : internal::StateSetupBase {
  SideStateSetup(entt::registry& registry) : SideStateSetup(registry, registry.create()) {}
  SideStateSetup(entt::registry& registry, entt::entity entity);

  /**
   * @brief Applies the defaults to the required properties for a player side's state.
   *
   * Some of the required properties are a blank `Battle`, `Side`, and `FoeSide` component.
   */
  /*_inline_*/ void initBlank();

  /*_inline_*/ void setTeam(std::vector<PokemonStateSetup>& team);
  /*_inline_*/ void setOpponent(entt::entity entity);
  /*_inline_*/ void setBattle(entt::entity entity);
};
}  // namespace pokesim