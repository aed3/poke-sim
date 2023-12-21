#pragma once

#include <Types/Entity.hpp>
#include <vector>

namespace pokesim {
// Contains a list of entities of the moves a Pokemon known.
struct MoveSlots {
  std::vector<types::entity> moveSlots{};
};
}  // namespace pokesim