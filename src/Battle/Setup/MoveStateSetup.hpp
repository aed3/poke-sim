#pragma once

#include <Types/Move.hpp>

#include "StateSetupBase.hpp"

namespace pokesim {
struct MoveStateSetup : internal::StateSetupBase {
  MoveStateSetup(entt::registry& registry) : StateSetupBase(registry, registry.create()) {}
  MoveStateSetup(entt::registry& registry, entt::entity entity) : StateSetupBase(registry, entity) {}

  /*_inline_*/ void initBlank();

  /*_inline_*/ void setName(dex::Move moveName);
  /*_inline_*/ void setPP(std::uint8_t pp);
  /*_inline_*/ void setMaxPP(std::uint8_t maxPP);
};
}  // namespace pokesim