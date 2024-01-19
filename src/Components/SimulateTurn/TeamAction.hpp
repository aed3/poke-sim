#pragma once

#include <Types/State.hpp>

namespace pokesim::action {
// Action Tag: When team member order has been picked as part of team preview
struct Team {
  types::teamPositions<types::teamPositionIndex> teamOrder;
};
}  // namespace pokesim::action