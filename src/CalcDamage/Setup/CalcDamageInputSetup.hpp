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
  types::registry& registry;
  types::entity moveEntity = entt::null;

 public:
  InputSetup(types::registry& registry);

  void setup(
    types::entity battleEntity, types::entity sourceEntity, types::entity targetEntity, dex::Move move,
    const Pokedex& pokedex);

  types::entity entity() const;
};
}  // namespace calc_damage
}  // namespace pokesim