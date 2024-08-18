#pragma once

#include <Types/Entity.hpp>
#include <Types/State.hpp>
#include <vector>

namespace pokesim {
struct CurrentAction {
  types::entity val{};
};

struct NextAction {
  types::entity val{};
};

struct CurrentActionTargets {
  types::targets<types::entity> val{};
};

struct CurrentActionSource {
  types::entity val{};
};

struct CurrentActionMoves {
  std::vector<types::entity> val{};
};

struct CurrentActionMoveSlot {
  types::entity val{};
};
}  // namespace pokesim