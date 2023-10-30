#pragma once

#include <entt/entity/fwd.hpp>
#include <vector>

namespace pokesim {
// Contains the list of action entities queued up to be simulated for a battle's current turn.
struct ActionQueue {
  std::vector<entt::entity> actionQueue{};
};
}  // namespace pokesim