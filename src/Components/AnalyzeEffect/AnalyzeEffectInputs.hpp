#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Random.hpp>
#include <Types/State.hpp>
#include <Utilities/MaxSizedVector.hpp>
#include <vector>

namespace pokesim::analyze_effect {
struct EffectTarget {
  types::entity val{};
};

struct EffectMove {
  dex::Move val = dex::Move::NO_MOVE;
};

struct MovesAndInputs {
 private:
  struct MoveAndInput {
    dex::Move move = dex::Move::NO_MOVE;
    types::entity input{};
  };

 public:
  internal::maxSizedVector<MoveAndInput> val{};

  bool operator==(const MovesAndInputs& other) const {
    if (val.size() != other.val.size()) return false;
    for (types::entityIndex i = 0U; i < val.size(); i++) {
      if (val[i].move != other.val[i].move) {
        return false;
      }
      if (val[i].input != other.val[i].input) {
        return false;
      }
    }
    return true;
  }
};

struct Inputs {
  types::entityVector val{};
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
 private:
  struct MovePair {
    types::entity original;
    types::entity copy;
    types::entity originInput;
  };

 public:
  internal::maxSizedVector<MovePair> val{};

  bool operator==(const MovePairs& other) const {
    if (val.size() != other.val.size()) return false;
    for (types::entityIndex i = 0U; i < val.size(); i++) {
      if (val[i].original != other.val[i].original) {
        return false;
      }
      if (val[i].copy != other.val[i].copy) {
        return false;
      }
      if (val[i].originInput != other.val[i].originInput) {
        return false;
      }
    }
    return true;
  }
};

struct SkippedInputCount {
  types::eventPossibilities val = 0U;
};

namespace tags {
struct Input {};
struct GroupedWithOtherInput {};
struct RunOneCalculation {};
struct BattleCloneForCalculation {};
struct InvertFinalAnswer {};
}  // namespace tags
}  // namespace pokesim::analyze_effect