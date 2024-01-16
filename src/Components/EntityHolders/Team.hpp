#pragma once

#include <Types/Entity.hpp>
#include <Types/State.hpp>

namespace pokesim {
// Contains a list of entities pointing to the Pokemon on a team.
struct Team {
  types::teamPositions<types::entity> team{};
};
}  // namespace pokesim