#pragma once

#include <Types/Constants.hpp>
#include <Types/Enums/DamageRollKind.hpp>
#include <Types/Random.hpp>
#include <Types/State.hpp>
#include <entt/signal/delegate.hpp>
#include <optional>
#include <type_traits>

namespace pokesim {
class Simulation;

struct DamageRollOptions {
 private:
  DamageRollKind p1 = DamageRollKind::AVERAGE_DAMAGE;
  DamageRollKind p2 = DamageRollKind::AVERAGE_DAMAGE;

 public:
  DamageRollOptions() {}
  DamageRollOptions(DamageRollKind p1AndP2) : p1(p1AndP2), p2(p1AndP2) {}
  DamageRollOptions(DamageRollKind p1_, DamageRollKind p2_) : p1(p1_), p2(p2_) {}

  constexpr DamageRollKind getP1() const { return p1; }
  constexpr DamageRollKind getP2() const { return p2; }

  DamageRollOptions& setP1(DamageRollKind newValue) {
    p1 = newValue;
    return *this;
  }
  DamageRollOptions& setP2(DamageRollKind newValue) {
    p2 = newValue;
    return *this;
  }
  bool sidesMatch() const { return p1 == p2; }
  bool operator==(const DamageRollOptions& other) const { return p1 == other.p1 && p2 == other.p2; }
};

namespace simulate_turn {
struct Options {
 private:
  DamageRollOptions damageRollsConsidered;
  bool applyChangesToInputBattle = true;
  std::optional<types::percentChance> randomChanceUpperLimit = std::nullopt;
  std::optional<types::percentChance> randomChanceLowerLimit = std::nullopt;
  std::optional<types::probability> branchProbabilityLowerLimit = std::nullopt;

  // For Monte Carlo method. When true, the number of branches is determined by the number of random chance events that
  // happen in the turn. When false, the number of output battles is the same as the number in input battles as each
  // battle picks a random outcome for each random event without branching.
  bool makeBranchesOnRandomEvents = true;

 public:
  constexpr const DamageRollOptions& getDamageRollsConsidered() const { return damageRollsConsidered; }
  constexpr bool getApplyChangesToInputBattle() const { return applyChangesToInputBattle; }
  constexpr types::percentChance getRandomChanceUpperLimit() const {
    return randomChanceUpperLimit.value_or(Constants::PercentChance::MAX);
  }
  constexpr types::percentChance getRandomChanceLowerLimit() const {
    return randomChanceLowerLimit.value_or(Constants::PercentChance::MIN);
  }
  constexpr types::probability getBranchProbabilityLowerLimit() const {
    return branchProbabilityLowerLimit.value_or(Constants::Probability::MIN);
  }
  constexpr bool hasBranchProbabilityLowerLimit() const { return branchProbabilityLowerLimit.has_value(); }
  constexpr bool getMakeBranchesOnRandomEvents() const { return makeBranchesOnRandomEvents; }

  Options& setDamageRollsConsidered(const DamageRollOptions& newValue) {
    damageRollsConsidered = newValue;
    return *this;
  }
  Options& setApplyChangesToInputBattle(bool newValue) {
    applyChangesToInputBattle = newValue;
    return *this;
  }
  Options& setRandomChanceUpperLimit(types::percentChance newValue) {
    randomChanceUpperLimit = newValue;
    return *this;
  }
  Options& setRandomChanceLowerLimit(types::percentChance newValue) {
    randomChanceLowerLimit = newValue;
    return *this;
  }
  Options& setBranchProbabilityLowerLimit(types::probability newValue) {
    branchProbabilityLowerLimit = newValue;
    return *this;
  }
  Options& setMakeBranchesOnRandomEvents(bool newValue) {
    makeBranchesOnRandomEvents = newValue;
    return *this;
  }

  entt::delegate<std::remove_pointer_t<types::callback>> decisionCallback{};
  entt::delegate<std::remove_pointer_t<types::callback>> faintCallback{};

  bool operator==(const Options& other) const {
    return damageRollsConsidered == other.damageRollsConsidered &&
           applyChangesToInputBattle == other.applyChangesToInputBattle &&
           randomChanceLowerLimit == other.randomChanceLowerLimit &&
           randomChanceUpperLimit == other.randomChanceUpperLimit &&
           branchProbabilityLowerLimit == other.branchProbabilityLowerLimit &&
           makeBranchesOnRandomEvents == other.makeBranchesOnRandomEvents &&
           decisionCallback == other.decisionCallback && faintCallback == other.faintCallback;
  }
};
}  // namespace simulate_turn

namespace calc_damage {
struct Options {
 private:
  bool calculateUpToFoeHp = false;
  // KO chance is otherwise calculated if DamageRollKind::ALL_DAMAGE_ROLLS is set as the damage roll option
  bool noKoChanceCalculation = false;

  DamageRollOptions damageRollOptions;

 public:
  constexpr bool getCalculateUpToFoeHp() const { return calculateUpToFoeHp; }
  constexpr bool getNoKoChanceCalculation() const { return noKoChanceCalculation; }
  constexpr const DamageRollOptions& getDamageRollOptions() const { return damageRollOptions; }

  Options& setCalculateUpToFoeHp(bool newValue) {
    calculateUpToFoeHp = newValue;
    return *this;
  }
  Options& setNoKoChanceCalculation(bool newValue) {
    noKoChanceCalculation = newValue;
    return *this;
  }
  Options& setDamageRollOptions(const DamageRollOptions& newValue) {
    damageRollOptions = newValue;
    return *this;
  }

  bool operator==(const Options& other) const {
    return calculateUpToFoeHp == other.calculateUpToFoeHp && noKoChanceCalculation == other.noKoChanceCalculation &&
           damageRollOptions == other.damageRollOptions;
  }
};
}  // namespace calc_damage

namespace analyze_effect {
struct Options {
 private:
  // Whether to consider the multiplier even if the effect is already active (i.e. Rain will return a 1x multiplier
  // instead of 1.5x multiplier for Surf if this option is true and it's already raining)
  bool reconsiderActiveEffects = false;
  bool calculateUpToFoeHp = false;
  // KO chance is otherwise calculated if DamageRollKind::ALL_DAMAGE_ROLLS is set as the damage roll option
  bool noKoChanceCalculation = false;

  DamageRollOptions damageRollOptions;

 public:
  constexpr bool getReconsiderActiveEffects() const { return reconsiderActiveEffects; }
  constexpr bool getCalculateUpToFoeHp() const { return calculateUpToFoeHp; }
  constexpr bool getNoKoChanceCalculation() const { return noKoChanceCalculation; }
  constexpr const DamageRollOptions& getDamageRollOptions() const { return damageRollOptions; }

  Options& setReconsiderActiveEffects(bool newValue) {
    reconsiderActiveEffects = newValue;
    return *this;
  }
  Options& setCalculateUpToFoeHp(bool newValue) {
    calculateUpToFoeHp = newValue;
    return *this;
  }
  Options& setNoKoChanceCalculation(bool newValue) {
    noKoChanceCalculation = newValue;
    return *this;
  }
  Options& setDamageRollOptions(const DamageRollOptions& newValue) {
    damageRollOptions = newValue;
    return *this;
  }

  bool operator==(const Options& other) const {
    return reconsiderActiveEffects == other.reconsiderActiveEffects && calculateUpToFoeHp == other.calculateUpToFoeHp &&
           noKoChanceCalculation == other.noKoChanceCalculation && damageRollOptions == other.damageRollOptions;
  }
};
}  // namespace analyze_effect
}  // namespace pokesim
