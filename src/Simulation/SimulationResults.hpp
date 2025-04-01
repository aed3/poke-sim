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
struct UsesUntilKo;
struct AttackerHpRecovered;
struct AttackerHpLost;

struct Results {
  types::view<DamageRolls> damageRollResults() const;
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
using MultipliedDamageRolls = DamageRolls;
using MultipliedUsesUntilKo = calc_damage::UsesUntilKo;

struct Results {
  types::view<EffectMultiplier> effectMultiplierResults() const;
  types::view<MultipliedDamageRolls> multipliedDamageRollsResults() const;
  types::view<MultipliedUsesUntilKo> multipliedUsesUntilKoResults() const;

  Results(const Simulation& simulation_);

 private:
  const Simulation& simulation;
};
}  // namespace analyze_effect
}  // namespace pokesim