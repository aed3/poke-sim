#pragma once

#include <Types/Entity.hpp>
#include <Types/State.hpp>

namespace pokesim {
// Contains a list of entities of the moves a Pokemon known.
struct MoveSlots {
  types::moveSlots<types::entity> val{};
};
}  // namespace pokesim