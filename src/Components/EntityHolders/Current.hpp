#pragma once

#include <Config/Require.hpp>
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
  const types::entity& only() const {
    POKESIM_ASSERT(
      val.size() == 1,
      "This method is supposed to get the first target when there's only one target stored.");
    return val[0];
  };
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