#pragma once

#include <Types/Damage.hpp>
#include <Types/Move.hpp>
#include <array>
#include <entt/container/dense_map.hpp>
#include <entt/entity/entity.hpp>
#include <entt/entity/view.hpp>
#include <vector>

namespace pokesim {
namespace simulate_turn {
struct TurnOutcomeBattles {
  std::vector<entt::entity> turnOutcomeBattles;
};

struct Results {
  std::vector<TurnOutcomeBattles> turnOutcomeBattlesResults() const;
};
}  // namespace simulate_turn

namespace calc_damage {
struct MaxDamage {
  types::Damage maxDamage = 0;
};

struct MinUsesUntilKo {
  types::Damage minUsesUntilKo = 0;
};

struct HpRecovered {
  types::Damage hpRecovered = 0;
};

struct HpLost {
  types::Damage hpLost = 0;
};

struct HitCount {
  types::MoveHits hitCount = 1;
};

struct Results {
  std::vector<MaxDamage> maxDamageResults() const;
  std::vector<MinUsesUntilKo> minUsesUntilKoResults() const;
  std::vector<HpRecovered> hpRecoveredResults() const;
  std::vector<HpLost> hpLostResults() const;
  std::vector<HitCount> hitCountResults() const;
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
  std::vector<EffectMultiplier> effectMultiplierResults() const;
  std::vector<MultipliedMaxDamage> multipliedMaxDamageResults() const;
  std::vector<MultipliedDamageRolls> multipliedDamageRollsResults() const;
  std::vector<MultipliedKoChance> multipliedKoChanceResults() const;
};
}  // namespace analyze_effect
}  // namespace pokesim