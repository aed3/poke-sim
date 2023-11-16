#pragma once

#include <array>
#include <cstdint>
#include <entt/container/dense_map.hpp>
#include <entt/entity/entity.hpp>
#include <entt/entity/view.hpp>
#include <vector>

namespace pokesim {
namespace simulate_turn {
struct TurnOutcomeBattles {
  std::vector<entt::entity> turnOutcomeBattles;
};
}  // namespace simulate_turn

struct SimulateTurnResults {
  std::vector<simulate_turn::TurnOutcomeBattles> turnOutcomeBattlesResults() const;
};

namespace calc_damage {
struct MaxDamage {
  std::uint16_t maxDamage;
};

struct MinUsesUntilKo {
  std::uint16_t minUsesUntilKo;
};

struct HpRecovered {
  std::uint16_t hpRecovered;
};

struct HpLost {
  std::uint16_t hpLost;
};

struct HitCount {
  std::uint8_t hitCount;
};
}  // namespace calc_damage

struct CalculateDamageResults {
  std::vector<calc_damage::MaxDamage> maxDamageResults() const;
  std::vector<calc_damage::MinUsesUntilKo> minUsesUntilKoResults() const;
  std::vector<calc_damage::HpRecovered> hpRecoveredResults() const;
  std::vector<calc_damage::HpLost> hpLostResults() const;
  std::vector<calc_damage::HitCount> hitCountResults() const;
};

namespace analyze_effect {
struct EffectMultiplier {
  float effectMultiplier = 1.0F;
};

struct MultipliedMaxDamage {
  std::uint16_t multipliedMaxDamage;
};

struct MultipliedDamageRolls {
  std::array<std::uint16_t, 16> multipliedDamageRolls;
};

struct MultipliedKoChance {
  float multipliedKoChance = 1.0F;
};
}  // namespace analyze_effect

struct AnalyzeEffectResults {
  std::vector<analyze_effect::EffectMultiplier> effectMultiplierResults() const;
  std::vector<analyze_effect::MultipliedMaxDamage> multipliedMaxDamageResults() const;
  std::vector<analyze_effect::MultipliedDamageRolls> multipliedDamageRollsResults() const;
  std::vector<analyze_effect::MultipliedKoChance> multipliedKoChanceResults() const;
};
}  // namespace pokesim