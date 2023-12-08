#pragma once

#include <entt/entity/fwd.hpp>

namespace pokesim {
// Contains the entity pointing to the player 1 or player 2 side of a battle.
struct Side {
  entt::entity side;
};
}  // namespace pokesim