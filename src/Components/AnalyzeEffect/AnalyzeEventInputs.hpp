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

struct OriginalInputEntities {
  types::entity battle{};
  types::entity attacker{};
  types::entity defender{};
  types::entity effectTarget{};

  bool operator==(const OriginalInputEntities& other) const {
    return battle == other.battle && attacker == other.attacker && defender == other.defender &&
           effectTarget == other.effectTarget;
  }
};

struct MovePairs {
  std::vector<std::pair<types::entity, types::entity>> val{};

  bool operator==(const MovePairs& other) const {
    if (val.size() != other.val.size()) return false;
    for (std::size_t i = 0; i < val.size(); i++) {
      if (val[i].first != other.val[i].first || val[i].second != other.val[i].second) {
        return false;
      }
    }
    return true;
  }
};

namespace tags {
struct Input {};
struct BattleCloneForCalculation {};
struct InvertFinalAnswer {};
}  // namespace tags
}  // namespace pokesim::analyze_effect