#pragma once

#include <Types/Entity.hpp>
#include <array>

namespace pokesim {
// Contains the entities pointing to the two sides of a battle.
struct Sides {
  std::array<types::entity, 2U> val{};

  types::entity& p1() { return val[0]; };
  types::entity& p2() { return val[1]; };
  const types::entity& p1() const { return val[0]; };
  const types::entity& p2() const { return val[1]; };
};
}  // namespace pokesim