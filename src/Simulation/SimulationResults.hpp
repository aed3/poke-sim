#pragma once

#include <Types/Damage.hpp>
#include <Types/Entity.hpp>
#include <Types/Move.hpp>
#include <Types/Stats.hpp>
#include <array>
#include <entt/container/dense_map.hpp>
#include <entt/entity/view.hpp>
#include <vector>

namespace pokesim {
class Simulation;

namespace simulate_turn {
struct TurnOutcomeBattles {
  std::vector<types::entity> turnOutcomeBattles;
};

struct Results {
  types::view<TurnOutcomeBattles> turnOutcomeBattlesResults() const;

  Results(const Simulation& simulation_);

 private:
  const Simulation& simulation;
};
}  // namespace simulate_turn

namespace calc_damage {
struct MaxDamage {
  types::damage maxDamage = 0;
};

struct MinUsesUntilKo {
  types::damage minUsesUntilKo = 0;
};

struct AttackerHpRecovered {
  types::stat hpRecovered = 0;
};

struct AttackerHpLost {
  types::stat hpLost = 0;
};

struct HitCount {
  types::moveHits hitCount = 1;
};

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
struct EffectMultiplier {
  float effectMultiplier = 1.0F;
};

struct MultipliedMaxDamage {
  types::damage multipliedMaxDamage;
};

struct MultipliedDamageRolls {
  types::damageRolls multipliedDamageRolls;
};

struct MultipliedKoChance {
  float multipliedKoChance = 1.0F;
};

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