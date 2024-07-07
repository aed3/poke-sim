#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Move.hpp>
#include <Types/Registry.hpp>
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
  void initBlank();

  void setName(dex::Move moveName);
  void setPP(types::pp pp);
  void setMaxPP(types::pp maxPp);
};
}  // namespace pokesim