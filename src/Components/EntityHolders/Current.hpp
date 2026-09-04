#pragma once

#include <Types/Constants.hpp>
#include <Types/Entity.hpp>
#include <Types/FixedMemoryVector.hpp>
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
  types::targets<types::entity> val{};

  auto begin() const noexcept { return val.cbegin(); }
  auto end() const noexcept { return val.cend(); }
};

struct CurrentActionMovesAsTarget {
  types::entity val{};

  auto begin() const noexcept { return &val; }
  auto end() const noexcept { return std::next(begin(), 1U); }
};

struct CurrentActionMovesAsSourceExtended {
  types::entityVector val{};

  auto begin() const noexcept { return val.cbegin(); }
  auto end() const noexcept { return val.cend(); }
};

struct CurrentActionMovesAsTargetExtended {
  types::entityVector val{};

  auto begin() const noexcept { return val.cbegin(); }
  auto end() const noexcept { return val.cend(); }
};

struct CurrentEffectSource {
  types::entity val{};
};

struct CurrentEffectTarget {
  types::entity val{};
};

struct CurrentEffectsAsSource {
  types::targets<types::entity> val{};
};

struct CurrentEffectsAsTarget {
  types::entity val{};
};
}  // namespace pokesim
