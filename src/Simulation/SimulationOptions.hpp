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

  // For Monte Carlo method. If no number is given, the number of branches
  // is determined by the number of random chance events that happen in the turn.
  // To get just one random outcome (aka using the simulator to just run a game),
  // set the value to 1
  std::optional<std::uint32_t> numberOfSamples = std::nullopt;
  bool makeBranchesOnRandomEvents() const { return !numberOfSamples.has_value(); }

  entt::delegate<std::remove_pointer_t<types::callback>> decisionCallback{};
  entt::delegate<std::remove_pointer_t<types::callback>> faintCallback{};
};
}  // namespace simulate_turn

namespace calc_damage {
struct Options {
  DamageRollOptions damageRollsReturned;
  bool calculateUpToFoeHp = false;
};
}  // namespace calc_damage

namespace analyze_effect {
struct Options {
  // Whether to consider the multiplier even if the effect is already active (i.e. Rain will return a 1x multiplier
  // instead of 1.5x multiplier for Surf if this option is true and it's already raining)
  bool reconsiderActiveEffects = false;
  bool returnMultipliedKoChance = false;

  DamageRollOptions damageRollsReturned;
};
}  // namespace analyze_effect
}  // namespace pokesim