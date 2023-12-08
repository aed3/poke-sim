#pragma once

#include <Types/Damage.hpp>
#include <Types/Entity.hpp>
#include <Types/Move.hpp>
#include <Types/Stats.hpp>
#include <array>
#include <entt/container/dense_map.hpp>
#include <entt/entity/entity.hpp>
#include <entt/entity/view.hpp>
#include <vector>

namespace pokesim {
class Simulation;

namespace simulate_turn {
struct TurnOutcomeBattles {
  std::vector<entt::entity> turnOutcomeBattles;
};

struct Results {
  /*_inline_*/ types::view<TurnOutcomeBattles> turnOutcomeBattlesResults() const;

  /*_inline_*/ Results(const Simulation& simulation_);

 private:
  const Simulation& simulation;
};
}  // namespace simulate_turn

namespace calc_damage {
struct MaxDamage {
  types::Damage maxDamage = 0;
};

struct MinUsesUntilKo {
  types::Damage minUsesUntilKo = 0;
};

struct AttackerHpRecovered {
  types::Stat hpRecovered = 0;
};

struct AttackerHpLost {
  types::Stat hpLost = 0;
};

struct HitCount {
  types::MoveHits hitCount = 1;
};

struct Results {
  /*_inline_*/ types::view<MaxDamage> maxDamageResults() const;
  /*_inline_*/ types::view<MinUsesUntilKo> minUsesUntilKoResults() const;
  /*_inline_*/ types::view<AttackerHpRecovered> hpRecoveredResults() const;
  /*_inline_*/ types::view<AttackerHpLost> hpLostResults() const;
  /*_inline_*/ types::view<HitCount> hitCountResults() const;

  /*_inline_*/ Results(const Simulation& simulation_);

 private:
  const Simulation& simulation;
};
}  // namespace calc_damage

namespace analyze_effect {
struct EffectMultiplier {
  float effectMultiplier = 1.0F;
};

struct MultipliedMaxDamage {
  types::Damage multipliedMaxDamage;
};

struct MultipliedDamageRolls {
  types::DamageRolls multipliedDamageRolls;
};

struct MultipliedKoChance {
  float multipliedKoChance = 1.0F;
};

struct Results {
  /*_inline_*/ types::view<EffectMultiplier> effectMultiplierResults() const;
  /*_inline_*/ types::view<MultipliedMaxDamage> multipliedMaxDamageResults() const;
  /*_inline_*/ types::view<MultipliedDamageRolls> multipliedDamageRollsResults() const;
  /*_inline_*/ types::view<MultipliedKoChance> multipliedKoChanceResults() const;

  /*_inline_*/ Results(const Simulation& simulation_);

 private:
  const Simulation& simulation;
};
}  // namespace analyze_effect
}  // namespace pokesim