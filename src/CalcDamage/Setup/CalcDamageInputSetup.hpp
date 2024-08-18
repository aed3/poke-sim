#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
class Pokedex;

namespace calc_damage {
struct InputSetup {
 protected:
  types::handle handle;

 public:
  InputSetup(
    types::registry& registry, types::entity battleEntity, types::entity sourceEntity, types::entity targetEntity,
    dex::Move move, const Pokedex& pokedex);

  types::entity entity() const;
};
}  // namespace calc_damage
}  // namespace pokesim