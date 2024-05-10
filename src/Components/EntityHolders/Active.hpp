#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
struct ActiveAction {
  types::entity activeAction;
};

struct NextAction {
  types::entity nextAction;
};

struct ActiveMove {
  types::entity activeMove;
};

struct ActiveTarget {
  types::entity activeTarget;
};

struct ActiveSource {
  types::entity activeSource;
};
}  // namespace pokesim