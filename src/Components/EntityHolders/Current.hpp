#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
struct CurrentAction {
  types::entity currentAction;
};

struct NextAction {
  types::entity nextAction;
};

struct CurrentActionTarget {
  types::entity actionTarget;
};

struct CurrentActionSource {
  types::entity actionSource;
};

struct CurrentActionMove {
  types::entity actionMove;
};
}  // namespace pokesim