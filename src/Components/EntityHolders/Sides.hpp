#pragma once

#include <Types/Entity.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/State.hpp>

namespace pokesim {
// Contains the entities pointing to the two sides of a battle.
struct Sides {
  types::sides<types::entity> val{};
};
}  // namespace pokesim
