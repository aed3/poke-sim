#pragma once

#include <entt/entity/fwd.hpp>
#include <vector>

namespace pokesim {
// Contains a list of entities of the moves a Pokemon known
struct MoveSlots {
  std::vector<entt::entity> moveSlots{};
};
}  // namespace pokesim