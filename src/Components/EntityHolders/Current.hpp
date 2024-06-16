#pragma once

#include <Types/Entity.hpp>
#include <Types/State.hpp>

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

struct CurrentActionMove {
  types::entity val{};
};

struct CurrentActionMoveSlot {
  types::entity val{};
};
}  // namespace pokesim