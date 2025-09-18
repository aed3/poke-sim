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
};

struct CurrentActionTarget {
  types::entity val{};
};

struct CurrentActionSource {
  types::entity val{};
};

struct CurrentActionMovesAsTarget {
  types::entityVector val{};
};

struct CurrentActionMovesAsSource {
  types::entityVector val{};
};

struct CurrentActionMoveSlot {
  types::entity val{};
};

struct CurrentEffectTarget {
  types::entity val{};
};

struct CurrentEffectSource {
  types::entity val{};
};

struct CurrentEffectsAsTarget {
  types::entityVector val{};
};

struct CurrentEffectsAsSource {
  types::entityVector val{};
};

}  // namespace pokesim