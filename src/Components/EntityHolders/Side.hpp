#pragma once

#include <entt/entity/fwd.hpp>

namespace pokesim {
struct Side {
  enum PlayerSideID : uint8_t {
    P1 = 0,
    P2 = 1,
  };

  entt::entity side;
};
}  // namespace pokesim