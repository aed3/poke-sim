#pragma once

#include <cstdint>
#include <entt/signal/delegate.hpp>
#include <optional>

namespace pokesim {
class Simulation;

enum DamageRollLimit : std::uint8_t {
  AVERAGE_DAMAGE = 0b00000000,
  ALL_DAMAGE_ROLES = 0b00000001,
  P1_MAX_DAMAGE = 0b00000010,
  P2_MAX_DAMAGE = 0b00000100,
  P1_MIN_DAMAGE = 0b00001000,
  P2_MIN_DAMAGE = 0b00010000,

  MAX_DAMAGE = P1_MAX_DAMAGE | P2_MAX_DAMAGE,
  MIN_DAMAGE = P1_MIN_DAMAGE | P2_MIN_DAMAGE,
};

struct SimulateTurnOptions {
  DamageRollLimit damageRollsConsidered = AVERAGE_DAMAGE;
  float randomChanceUpperLimit = 0.9F;
  float randomChanceLowerLimit = 0.1F;
  float branchProbabilityLowerLimit = 0.0F;

  // For Monte Carlo method. If no number is given, the number of branches
  // is determined by the number of random chance events that happen in the turn.
  // To get just one random outcome (aka using the simulator to just run a game),
  // set the value to 1
  std::optional<std::uint32_t> numberOfSamples = std::nullopt;

  entt::delegate<void(Simulation&)> decisionCallback{};
  entt::delegate<void(Simulation&)> faintCallback{};
};

struct CalculateDamageOptions {
  DamageRollLimit damageRollsReturned = ALL_DAMAGE_ROLES;
};

struct AnalyzeEffectOptions {
  // Whether to consider the multiplier even if the effect is already active (i.e. Rain will return a 1x multiplier
  // instead of 1.5x multiplier for Surf if this option is true and it's already raining)
  bool considerActiveEffects = false;
};

struct SimulateTurnResults {};
struct CalculateDamageResults {};
struct AnalyzeEffectResults {};
}  // namespace pokesim