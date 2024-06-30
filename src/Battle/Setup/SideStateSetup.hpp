#pragma once

#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <entt/entity/registry.hpp>
#include <vector>

#include "StateSetupBase.hpp"

namespace pokesim {
struct PokemonStateSetup;

// Tool to set properties of a player's side state to an entity.
struct SideStateSetup : internal::StateSetupBase {
  SideStateSetup(types::registry& registry) : SideStateSetup(registry, registry.create()) {}
  SideStateSetup(types::registry& registry, types::entity entity) : StateSetupBase(registry, entity) {}
  /**
   * @brief Applies the defaults to the required properties for a player side's state.
   *
   * Some of the required properties are a blank `Battle`, `Side`, and `FoeSide` component.
   */
  void initBlank();

  void setTeam(std::vector<PokemonStateSetup>& team);
  void setOpponent(types::entity entity);
  void setBattle(types::entity entity);
};
}  // namespace pokesim