#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
// Contains the entity pointing to the player 1 or player 2 side of a battle.
struct Side {
  types::entity val{};
};
}  // namespace pokesim