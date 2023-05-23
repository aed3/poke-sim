#pragma once

#include <Types/Move.hpp>

#include "Setup.hpp"

namespace pokesim {
struct MoveSetup : internal::BattleStateSetup {
  MoveSetup(entt::registry& registry) : BattleStateSetup(registry, registry.create()) {}
  MoveSetup(entt::registry& registry, entt::entity entity) : BattleStateSetup(registry, entity) {}

  /*_inline_*/ void initBlank();

  /*_inline*/ void setName(dex::Move moveName);
  /*_inline*/ void setPP(std::uint8_t pp);
  /*_inline*/ void setMaxPP(std::uint8_t maxPP);
};
}  // namespace pokesim