#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::calc_damage {
struct InputSetup {
 protected:
  types::handle handle;

 public:
  InputSetup(types::registry& registry, types::entity entity) : handle(registry, entity) {}
  InputSetup(types::registry& registry) : InputSetup(registry, registry.create()) {}

  void setAttacker(types::entity entity);
  void setDefender(types::entity entity);
  void setMove(dex::Move move);
  void setBattle(types::entity entity);

  types::entity entity() { return handle.entity(); }
};
}  // namespace pokesim::calc_damage