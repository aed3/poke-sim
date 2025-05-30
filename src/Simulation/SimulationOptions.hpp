#pragma once

#include <Types/Enums/DamageRollKind.hpp>
#include <Types/Random.hpp>
#include <Types/State.hpp>
#include <cstdint>
#include <entt/signal/delegate.hpp>
#include <optional>
#include <type_traits>

namespace pokesim {
class Simulation;

struct DamageRollOptions {
  DamageRollKind p1 = DamageRollKind::AVERAGE_DAMAGE;
  DamageRollKind p2 = DamageRollKind::AVERAGE_DAMAGE;
  bool sidesMatch() const { return p1 == p2; }
};

namespace simulate_turn {
struct Options {
  DamageRollOptions damageRollsConsidered;
  bool applyChangesToInputBattle = true;
  std::optional<types::percentChance> randomChanceUpperLimit = std::nullopt;
  std::optional<types::percentChance> randomChanceLowerLimit = std::nullopt;
  std::optional<types::probability> branchProbabilityLowerLimit = std::nullopt;

  // For Monte Carlo method. When true, the number of branches is determined by the number of random chance events that
  // happen in the turn. When false, the number of output battles is the same as the number in input battles as each
  // battle picks a random outcome for each random event without branching.
  bool makeBranchesOnRandomEvents = true;

  entt::delegate<std::remove_pointer_t<types::callback>> decisionCallback{};
  entt::delegate<std::remove_pointer_t<types::callback>> faintCallback{};
};
}  // namespace simulate_turn

namespace calc_damage {
struct Options {
  bool calculateUpToFoeHp = false;
  // KO chance is otherwise calculated if DamageRollKind::ALL_DAMAGE_ROLLS is set as the damage roll option
  bool noKoChanceCalculation = false;

  DamageRollOptions damageRollOptions;
};
}  // namespace calc_damage

namespace analyze_effect {
struct Options {
  // Whether to consider the multiplier even if the effect is already active (i.e. Rain will return a 1x multiplier
  // instead of 1.5x multiplier for Surf if this option is true and it's already raining)
  bool reconsiderActiveEffects = false;
  bool calculateUpToFoeHp = false;
  // KO chance is otherwise calculated if DamageRollKind::ALL_DAMAGE_ROLLS is set as the damage roll option
  bool noKoChanceCalculation = false;

  DamageRollOptions damageRollOptions;
};
}  // namespace analyze_effect
}  // namespace pokesim