#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Move.hpp>
#include <entt/entity/registry.hpp>

#include "StateSetupBase.hpp"

namespace pokesim {
// Tool to set properties of a move's state to an entity.
struct MoveStateSetup : internal::StateSetupBase {
  MoveStateSetup(types::registry& registry) : MoveStateSetup(registry, registry.create()) {}
  MoveStateSetup(types::registry& registry, types::entity entity) : StateSetupBase(registry, entity) {}

  /**
   * @brief Applies the defaults to the required properties for a move state.
   *
   * Some of the required properties are a blank `MoveName`, `Pp`, and `MaxPp` component.
   */
  /*_inline_*/ void initBlank();

  /*_inline_*/ void setName(dex::Move moveName);
  /*_inline_*/ void setPP(types::pp pp);
  /*_inline_*/ void setMaxPP(types::pp maxPp);
};
}  // namespace pokesim