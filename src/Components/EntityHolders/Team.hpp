#pragma once

#include <entt/entity/fwd.hpp>
#include <vector>

namespace pokesim {
// Contains a list of entities pointing to the Pokemon on a team
struct Team {
  std::vector<entt::entity> team{};
};
}  // namespace pokesim