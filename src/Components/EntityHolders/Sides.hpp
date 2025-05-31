#pragma once

#include <Types/Entity.hpp>
#include <Types/MechanicConstants.hpp>
#include <array>

namespace pokesim {
// Contains the entities pointing to the two sides of a battle.
struct Sides {
  std::array<types::entity, MechanicConstants::SIDE_COUNT> val{};

  constexpr types::entity& p1() { return val[0]; };
  constexpr types::entity& p2() { return val[1]; };
  constexpr const types::entity& p1() const { return val[0]; };
  constexpr const types::entity& p2() const { return val[1]; };
};
}  // namespace pokesim