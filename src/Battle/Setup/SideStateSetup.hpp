#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/Indexes.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/registry.hpp>
#include <vector>

#include "StateSetupBase.hpp"

namespace pokesim {
struct SideDecision;

namespace internal {
struct PokemonStateSetup;
// Tool to set properties of a player's side state to an entity.
struct SideStateSetup : StateSetupBase {
  SideStateSetup() : StateSetupBase() {}
  SideStateSetup(types::registry& registry, types::entity entity);
  /**
   * @brief Applies the defaults to the required properties for a player side's state.
   *
   * Some of the required properties are a blank `Battle`, `Side`, and `FoeSide` component.
   */
  void initBlank();

  void setTeam(std::vector<PokemonStateSetup>& team);
  void setOpponent(types::entity entity, types::teamPositionIndex opponentTeamSize);
  void setBattle(types::entity entity);
  void setPlayerSide(PlayerSideId playerSideId);
  void setSideDecision(const SideDecision& sideDecision);
};
}  // namespace internal
}  // namespace pokesim
