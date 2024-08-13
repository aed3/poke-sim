#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
class Simulation;
struct HitCount;
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
struct MaxDamage;
struct MinUsesUntilKo;
struct AttackerHpRecovered;
struct AttackerHpLost;

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
struct EffectMultiplier;
using MultipliedDamage = Damage;
using MultipliedDamageRolls = DamageRolls;
struct MultipliedKoChance;

struct Results {
  types::view<EffectMultiplier> effectMultiplierResults() const;
  types::view<MultipliedDamage> multipliedMaxDamageResults() const;
  types::view<MultipliedDamageRolls> multipliedDamageRollsResults() const;
  types::view<MultipliedKoChance> multipliedKoChanceResults() const;

  Results(const Simulation& simulation_);

 private:
  const Simulation& simulation;
};
}  // namespace analyze_effect
}  // namespace pokesim