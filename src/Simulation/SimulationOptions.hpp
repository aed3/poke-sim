#pragma once

#include <Types/Enums/DamageRollKind.hpp>
#include <cstdint>
#include <entt/signal/delegate.hpp>
#include <optional>

namespace pokesim {
class Simulation;

namespace simulate_turn {
struct Options {
  types::DamageRollKind damageRollsConsidered = types::DamageRollKind::AVERAGE_DAMAGE;
  bool applyChangesToInputBattle = true;
  float randomChanceUpperLimit = 0.9F;  // NOLINT(readability-magic-numbers)
  float randomChanceLowerLimit = 0.1F;  // NOLINT(readability-magic-numbers)
  float branchProbabilityLowerLimit = 0.0F;

  // For Monte Carlo method. If no number is given, the number of branches
  // is determined by the number of random chance events that happen in the turn.
  // To get just one random outcome (aka using the simulator to just run a game),
  // set the value to 1
  std::optional<std::uint32_t> numberOfSamples = std::nullopt;

  entt::delegate<void(Simulation&)> decisionCallback{};
  entt::delegate<void(Simulation&)> faintCallback{};
};
}  // namespace simulate_turn

namespace calc_damage {
struct Options {
  types::DamageRollKind damageRollsReturned = types::DamageRollKind::ALL_DAMAGE_ROLES;
};
}  // namespace calc_damage

namespace analyze_effect {
struct Options {
  // Whether to consider the multiplier even if the effect is already active (i.e. Rain will return a 1x multiplier
  // instead of 1.5x multiplier for Surf if this option is true and it's already raining)
  bool reconsiderActiveEffects = false;
  bool returnMultipliedKoChance = false;

  types::DamageRollKind damageRollsReturned = types::DamageRollKind::NONE;
};
}  // namespace analyze_effect
}  // namespace pokesim