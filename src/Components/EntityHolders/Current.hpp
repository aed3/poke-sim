#pragma once

#include <Types/Entity.hpp>
#include <Types/State.hpp>

namespace pokesim {
struct CurrentAction {
  types::entity action{};
  types::entity source{};
  types::targets<types::entity> targets{};

  auto toList() const {
    types::fixedMemoryVector<types::entity, 2U + Constants::Targets::MAX> list{action, source};
    for (types::entity target : targets) {
      list.push_back(target);
    }
    return list;
  }
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
