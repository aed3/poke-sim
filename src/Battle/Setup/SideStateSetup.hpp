#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <entt/entity/registry.hpp>
#include <vector>

#include "StateSetupBase.hpp"

namespace pokesim {
struct PokemonStateSetup;

// Tool to set properties of a player's side state to an entity.
struct SideStateSetup : internal::StateSetupBase {
  SideStateSetup(types::registry& registry, PlayerSideId playerSideId)
      : SideStateSetup(registry, registry.create(), playerSideId) {}
  SideStateSetup(types::registry& registry, types::entity entity, PlayerSideId playerSideId);
  /**
   * @brief Applies the defaults to the required properties for a player side's state.
   *
   * Some of the required properties are a blank `Battle`, `Side`, and `FoeSide` component.
   */
  void initBlank();

  void setTeam(std::vector<PokemonStateSetup>& team);
  void setOpponent(types::entity entity);
  void setBattle(types::entity entity);
  void setPlayerSide(PlayerSideId playerSideId);
};
}  // namespace pokesim