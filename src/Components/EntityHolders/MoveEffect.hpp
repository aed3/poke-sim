#pragma once

#include <entt/entity/fwd.hpp>

namespace pokesim {
struct MoveEffect {
  bool primary = true;
  entt::entity moveEffect{};
};
}  // namespace pokesim