#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <vector>

namespace pokesim::analyze_effect {
struct EffectTarget {
  types::entity val{};
};

struct EffectMoves {
  std::vector<dex::Move> val{};
};

struct Inputs {
  std::vector<types::entity> val{};
};

struct MovePairs {
  std::vector<std::pair<types::entity, types::entity>> val{};
};

namespace tags {
struct Input {};
struct BattleCloneForCalculation {};
struct InvertFinalAnswer {};
}  // namespace tags
}  // namespace pokesim::analyze_effect