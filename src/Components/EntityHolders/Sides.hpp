#pragma once

#include <entt/entity/fwd.hpp>

namespace pokesim {
// Contains the entities pointing to the two sides of a battle
struct Sides {
  entt::entity p1;
  entt::entity p2;
};
}  // namespace pokesim