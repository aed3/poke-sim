#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
class Pokedex;

namespace internal::calc_damage {
struct InputSetup {
 private:
  types::handle handle;

 public:
  InputSetup(types::registry& registry, types::entity moveEntity);

  void setup(
    types::entity battleEntity, types::entity sourceEntity, types::entity targetEntity, pokesim::dex::Move move);

  types::entity entity() const { return handle.entity(); }
};
}  // namespace internal::calc_damage
}  // namespace pokesim
