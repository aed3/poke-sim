#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/State.hpp>
#include <entt/entity/registry.hpp>
#include <optional>
#include <vector>

#include "StateSetupBase.hpp"

namespace pokesim {
struct SimulateTurnOptions;
struct CalculateDamageOptions;
struct AnalyzeEffectOptions;

// Tool to set properties of a battle's state to an entity.
struct BattleStateSetup : internal::StateSetupBase {
  BattleStateSetup(types::registry& registry) : BattleStateSetup(registry, registry.create()) {}
  /*_inline_*/ BattleStateSetup(types::registry& registry, types::entity entity);

  /**
   * @brief Applies the defaults to the required properties for a battle state.
   *
   * @details Some of the required properties and their defaults:
   * - Turn: 0
   * - Probability: 1
   * - Id: The number of existing battle states
   * - Sides: Unassigned entities for P1 and P2
   * - ActionQueue: An empty queue
   */
  /*_inline_*/ void initBlank();

  /*_inline_*/ void setAutoID();
  /*_inline_*/ void setID(types::stateId id);
  /*_inline_*/ void setSide(PlayerSideId sideID, types::entity sideEntity);

  // If a seed is not provided, the seed is set to a random number based on the current time in nanoseconds.
  /*_inline_*/ void setRNGSeed(std::optional<types::stateRngSeed> seed = std::nullopt);
  /*_inline_*/ void setActionQueue(const std::vector<types::entity>& queue);
  /*_inline_*/ void setTurn(types::battleTurn turn);
  /*_inline_*/ void setActiveMove(types::entity activeMove);
  /*_inline_*/ void setActivePokemon(types::entity activePokemon);
  /*_inline_*/ void setActiveTarget(types::entity activeTarget);
  /*_inline_*/ void setActiveUser(types::entity activeSource);
  /*_inline_*/ void setProbability(types::stateProbability probability);

  /*_inline_*/ std::vector<BattleStateSetup> clone(std::optional<types::cloneIndex> cloneCount = std::nullopt);
};
}  // namespace pokesim