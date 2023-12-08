#pragma once

#include <Types/Enums/Move.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::calc_damage {
struct InputSetup {
 protected:
  entt::handle handle;

 public:
  InputSetup(entt::registry& registry, entt::entity entity) : handle(registry, entity) {}
  InputSetup(entt::registry& registry) : InputSetup(registry, registry.create()) {}

  /*_inline_*/ void setAttacker(entt::entity entity);
  /*_inline_*/ void setDefender(entt::entity entity);
  /*_inline_*/ void setMove(dex::Move move);
  /*_inline_*/ void setBattle(entt::entity entity);

  entt::entity entity() { return handle.entity(); }
};
}  // namespace pokesim::calc_damage