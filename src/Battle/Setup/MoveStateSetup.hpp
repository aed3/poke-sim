#pragma once

#include <Types/Move.hpp>
#include <cstdint>
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>

#include "StateSetupBase.hpp"

namespace pokesim {
// Tool to set properties of a move's state to an entity.
struct MoveStateSetup : internal::StateSetupBase {
  MoveStateSetup(entt::registry& registry) : StateSetupBase(registry, registry.create()) {}
  MoveStateSetup(entt::registry& registry, entt::entity entity) : StateSetupBase(registry, entity) {}

  /**
   * @brief Applies the defaults to the required properties for a move state.
   *
   * Some of the required properties are a blank `MoveName`, `PP`, and `MaxPP` component.
   */
  /*_inline_*/ void initBlank();

  /*_inline_*/ void setName(dex::Move moveName);
  /*_inline_*/ void setPP(std::uint8_t pp);
  /*_inline_*/ void setMaxPP(std::uint8_t maxPP);
};
}  // namespace pokesim