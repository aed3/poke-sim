#pragma once

#include <Types/Entity.hpp>
#include <Types/State.hpp>

namespace pokesim {
struct CurrentAction {
  types::entity currentAction;
};

struct NextAction {
  types::entity nextAction;
};

struct CurrentActionTargets {
  types::targets<types::entity> actionTargets;
};

struct CurrentActionSource {
  types::entity actionSource;
};

struct CurrentActionMove {
  types::entity actionMove;
};

struct CurrentActionMoveSlot {
  types::entity actionMoveSlot;
};
}  // namespace pokesim