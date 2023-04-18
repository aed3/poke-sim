#pragma once

#include <entt/entity/fwd.hpp>
#include <vector>

namespace pokesim {
struct ActionQueue {
  std::vector<entt::entity> actionQueue;
};
}  // namespace pokesim