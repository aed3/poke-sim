#pragma once

#include <Types/Entity.hpp>
#include <Types/Random.hpp>
#include <Types/Registry.hpp>
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
  BattleStateSetup(types::registry& registry, types::entity entity);

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
  void initBlank();

  void setAutoID();
  void setID(types::stateId id);
  void setSide(types::entity sideEntity);

  // If a seed is not provided, the seed is set to a random number based on the current time in nanoseconds.
  void setRNGSeed(std::optional<types::rngState> seed = std::nullopt);
  void setActionQueue(const std::vector<types::entity>& queue);
  void setTurn(types::battleTurn turn);
  void setCurrentActionTarget(types::targets<types::entity> actionTargets);
  void setCurrentActionSource(types::entity actionSource);
  void setCurrentActionMove(types::entity actionMove);
  void setProbability(types::probability probability);

  std::vector<BattleStateSetup> clone(std::optional<types::cloneIndex> cloneCount = std::nullopt);
};
}  // namespace pokesim