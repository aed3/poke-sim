#pragma once

#include <Types/State.hpp>
#include <entt/entity/fwd.hpp>

namespace pokesim {
// Contains a list of entities pointing to the Pokemon on a team.
struct Team {
  types::TeamPositions<entt::entity> team{};
};
}  // namespace pokesim