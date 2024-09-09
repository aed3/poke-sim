#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
class Simulation;
struct Damage;
struct DamageRolls;

namespace simulate_turn {
struct TurnOutcomeBattles;
struct Results {
  types::view<TurnOutcomeBattles> turnOutcomeBattlesResults() const;

  Results(const Simulation& simulation_);

 private:
  const Simulation& simulation;
};
}  // namespace simulate_turn

namespace calc_damage {
using MaxDamage = Damage;
struct UsesUntilKo;
struct AttackerHpRecovered;
struct AttackerHpLost;

struct Results {
  types::view<MaxDamage> maxDamageResults() const;
  types::view<UsesUntilKo> usesUntilKoResults() const;
  types::view<AttackerHpRecovered> hpRecoveredResults() const;
  types::view<AttackerHpLost> hpLostResults() const;

  Results(const Simulation& simulation_);

 private:
  const Simulation& simulation;
};
}  // namespace calc_damage

namespace analyze_effect {
struct EffectMultiplier;
using MultipliedDamage = Damage;
using MultipliedDamageRolls = DamageRolls;
using MultipliedUsesUntilKo = calc_damage::UsesUntilKo;

struct Results {
  types::view<EffectMultiplier> effectMultiplierResults() const;
  types::view<MultipliedDamage> multipliedMaxDamageResults() const;
  types::view<MultipliedDamageRolls> multipliedDamageRollsResults() const;
  types::view<MultipliedUsesUntilKo> multipliedUsesUntilKoResults() const;

  Results(const Simulation& simulation_);

 private:
  const Simulation& simulation;
};
}  // namespace analyze_effect
}  // namespace pokesim