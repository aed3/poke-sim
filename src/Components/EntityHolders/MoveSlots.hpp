#pragma once

#include <entt/entity/fwd.hpp>
#include <vector>

namespace pokesim {
struct MoveSlots {
  std::vector<entt::entity> moveSlots{};
};
}  // namespace pokesim