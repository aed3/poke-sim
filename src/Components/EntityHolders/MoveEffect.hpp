#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
// Contains the entity of what the primary or secondary effect of a move does.
struct MoveEffect {
  bool primary = true;
  types::entity moveEffect{};
};
}  // namespace pokesim