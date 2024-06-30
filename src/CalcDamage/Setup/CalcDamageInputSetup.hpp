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
  InputSetup(types::registry& registry, const Pokedex& pokedex, dex::Move move);

  void setAttacker(types::entity entity);
  void setDefender(types::entity entity);
  void setBattle(types::entity entity);

  types::entity entity() const;
};
}  // namespace calc_damage
}  // namespace pokesim