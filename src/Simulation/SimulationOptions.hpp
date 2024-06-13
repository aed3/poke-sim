#pragma once

#include <Types/Enums/DamageRollKind.hpp>
#include <Types/Random.hpp>
#include <cstdint>
#include <entt/signal/delegate.hpp>
#include <optional>

namespace pokesim {
class Simulation;

namespace simulate_turn {
struct Options {
  DamageRollKind damageRollsConsidered = DamageRollKind::AVERAGE_DAMAGE;
  bool applyChangesToInputBattle = true;
  types::percentChance randomChanceUpperLimit = 90;  // NOLINT(readability-magic-numbers)
  types::percentChance randomChanceLowerLimit = 10;  // NOLINT(readability-magic-numbers)
  types::probability branchProbabilityLowerLimit = 0.0F;

  // For Monte Carlo method. If no number is given, the number of branches
  // is determined by the number of random chance events that happen in the turn.
  // To get just one random outcome (aka using the simulator to just run a game),
  // set the value to 1
  std::optional<std::uint32_t> numberOfSamples = std::nullopt;
  bool makeBranchesOnRandomEvents() const { return !numberOfSamples.has_value(); }

  entt::delegate<void(Simulation&)> decisionCallback{};
  entt::delegate<void(Simulation&)> faintCallback{};
};
}  // namespace simulate_turn

namespace calc_damage {
struct Options {
  DamageRollKind damageRollsReturned = DamageRollKind::ALL_DAMAGE_ROLES;
};
}  // namespace calc_damage

namespace analyze_effect {
struct Options {
  // Whether to consider the multiplier even if the effect is already active (i.e. Rain will return a 1x multiplier
  // instead of 1.5x multiplier for Surf if this option is true and it's already raining)
  bool reconsiderActiveEffects = false;
  bool returnMultipliedKoChance = false;

  DamageRollKind damageRollsReturned = DamageRollKind::NONE;
};
}  // namespace analyze_effect
}  // namespace pokesim