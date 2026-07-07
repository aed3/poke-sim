#pragma once

#include <Types/Effect.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Stat.hpp>
#include <Types/Registry.hpp>
#include <Types/Stats.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::internal::analyze_effect {
struct InputSetup {
 private:
  types::handle handle;

 public:
  InputSetup(types::registry& registry, types::entity entity);

  void setAttacker(types::entity entity);
  void setEffectTarget(types::entity entity);
  void setDefender(types::entity entity);
  void setEffectMove(pokesim::dex::Move move);
  void setEffect(types::effectEnum effect);
  void setBoostEffect(pokesim::dex::Stat stat, types::boost boost);
  void setBattle(types::entity entity);

  types::entity entity() const { return handle.entity(); }
};
}  // namespace pokesim::internal::analyze_effect
