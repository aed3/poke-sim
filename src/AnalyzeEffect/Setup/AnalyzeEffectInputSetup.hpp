#pragma once

#include <Types/Effect.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Stat.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <vector>

namespace pokesim::analyze_effect {
struct InputSetup {
 protected:
  types::handle handle;

 public:
  InputSetup(types::registry& registry, types::entity entity);
  InputSetup(types::registry& registry) : InputSetup(registry, registry.create()) {}

  void setAttacker(types::entity entity);
  void setEffectTarget(types::entity entity);
  void setDefender(types::entity entity);
  void setEffectMoves(const std::vector<dex::Move>& moves);
  void setEffect(types::effectEnum effect);
  void setBoostEffect(dex::Stat stat, types::boost boost);
  void setBattle(types::entity entity);

  types::entity entity() { return handle.entity(); }
};
}  // namespace pokesim::analyze_effect