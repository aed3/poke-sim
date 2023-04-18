#pragma once

#include <entt/entity/fwd.hpp>
#include <vector>

namespace pokesim {
struct Team {
  std::vector<entt::entity> team;
};
}  // namespace pokesim