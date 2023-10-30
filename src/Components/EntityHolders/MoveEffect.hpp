#pragma once

#include <entt/entity/fwd.hpp>

namespace pokesim {
// Contains the entity of what the primary or secondary effect of a move does.
struct MoveEffect {
  bool primary = true;
  entt::entity moveEffect{};
};
}  // namespace pokesim