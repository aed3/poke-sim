#pragma once

#include <Components/EntityHolders/Side.hpp>
#include <cstdint>
#include <entt/entity/fwd.hpp>
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
  BattleStateSetup(entt::registry& registry) : StateSetupBase(registry, registry.create()) {}
  BattleStateSetup(entt::registry& registry, entt::entity entity) : StateSetupBase(registry, entity) {}

  /**
   * @brief Applies the defaults to the required properties for a battle state.
   *
   * @details Some of the required properties and their defaults:
   * - Turn: 0
   * - Probability: 1
   * - ID: The number of existing battle states
   * - Sides: Unassigned entities for P1 and P2
   * - ActionQueue: An empty queue
   */
  /*_inline_*/ void initBlank();

  /*_inline_*/ void setAutoID();
  /*_inline_*/ void setID(std::uint16_t id);
  /*_inline_*/ void setSide(Side::PlayerSideID sideID, entt::entity sideEntity);

  // If a seed is not provided, the seed is set to a random number based on the current time in nanoseconds.
  /*_inline_*/ void setRNGSeed(std::optional<std::uint32_t> seed = std::nullopt);
  /*_inline_*/ void setActionQueue(const std::vector<entt::entity>& queue);
  /*_inline_*/ void setTurn(std::uint16_t turn);
  /*_inline_*/ void setActiveMove(entt::entity activeMove);
  /*_inline_*/ void setActivePokemon(entt::entity activePokemon);
  /*_inline_*/ void setActiveTarget(entt::entity activeTarget);
  /*_inline_*/ void setActiveUser(entt::entity activeSource);
  /*_inline_*/ void setProbability(float probability);

  /*_inline_*/ void setSimulateTurnOptions(SimulateTurnOptions option);
  /*_inline_*/ void setCalculateDamageOptions(CalculateDamageOptions option);
  /*_inline_*/ void setAnalyzeEffectOptions(AnalyzeEffectOptions option);
};
}  // namespace pokesim