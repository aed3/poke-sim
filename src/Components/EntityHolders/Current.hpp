#pragma once

#include <Config/Require.hpp>
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

struct CurrentActionTarget {
  types::entity val{};
};

struct FailedCurrentActionSource {
  types::entity val{};
};

struct FailedCurrentActionTarget {
  types::entity val{};
};

struct CurrentActionMovesAsSource {
  types::entityVector val{};
};

struct CurrentActionMovesAsTarget {
  types::entityVector val{};
};

struct CurrentActionMoveSlot {
  types::entity val{};
};

struct CurrentEffectSource {
  types::entity val{};
};

struct CurrentEffectTarget {
  types::entity val{};
};

struct CurrentEffectsAsSource {
  types::entityVector val{};
};

struct CurrentEffectsAsTarget {
  types::entityVector val{};
};
}  // namespace pokesim
