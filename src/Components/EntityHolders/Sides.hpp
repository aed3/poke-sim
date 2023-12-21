#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
// Contains the entities pointing to the two sides of a battle.
struct Sides {
  types::entity p1;
  types::entity p2;
};
}  // namespace pokesim