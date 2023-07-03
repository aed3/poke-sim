#pragma once

#include <entt/entity/fwd.hpp>

namespace pokesim {
// Contains the entity pointing to the player 1 or player 2 side of a battle
struct Side {
  enum PlayerSideID : uint8_t {
    P1 = 0,
    P2 = 1,
  };

  entt::entity side;
};
}  // namespace pokesim