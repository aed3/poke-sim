#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/MaxSizedVector.hpp>
#include <Types/State.hpp>

namespace pokesim::analyze_effect {
struct EffectTarget {
  types::entity val{};
};

struct EffectMove {
  dex::Move val = dex::Move::NO_MOVE;
};

struct GroupedInputs {
  types::entityVector val{};
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

struct MovePair {
  types::entity original;
  types::entity copy;

  bool operator==(const MovePair& other) const { return original == other.original && copy == other.copy; }
};

struct SkippedInputCount {
  types::entityIndex val = 0U;
};

namespace tags {
struct Input {};
struct GroupedWithOtherInput {};
struct RunOneCalculation {};
struct BattleCloneForCalculation {};
struct InvertFinalAnswer {};
}  // namespace tags
}  // namespace pokesim::analyze_effect
