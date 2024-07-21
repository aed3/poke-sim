#pragma once

#include <Components/SimulationResults.hpp>

namespace pokesim {
class Simulation;

namespace simulate_turn {
struct Results {
  types::view<TurnOutcomeBattles> turnOutcomeBattlesResults() const;

  Results(const Simulation& simulation_);

 private:
  const Simulation& simulation;
};
}  // namespace simulate_turn

namespace calc_damage {
struct Results {
  types::view<MaxDamage> maxDamageResults() const;
  types::view<MinUsesUntilKo> minUsesUntilKoResults() const;
  types::view<AttackerHpRecovered> hpRecoveredResults() const;
  types::view<AttackerHpLost> hpLostResults() const;
  types::view<HitCount> hitCountResults() const;

  Results(const Simulation& simulation_);

 private:
  const Simulation& simulation;
};
}  // namespace calc_damage

namespace analyze_effect {
struct Results {
  types::view<EffectMultiplier> effectMultiplierResults() const;
  types::view<MultipliedMaxDamage> multipliedMaxDamageResults() const;
  types::view<MultipliedDamageRolls> multipliedDamageRollsResults() const;
  types::view<MultipliedKoChance> multipliedKoChanceResults() const;

  Results(const Simulation& simulation_);

 private:
  const Simulation& simulation;
};
}  // namespace analyze_effect
}  // namespace pokesim