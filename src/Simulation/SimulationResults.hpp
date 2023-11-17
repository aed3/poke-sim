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

struct Results {
  std::vector<TurnOutcomeBattles> turnOutcomeBattlesResults() const;
};
}  // namespace simulate_turn

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
  std::uint16_t multipliedMaxDamage;
};

struct MultipliedDamageRolls {
  std::array<std::uint16_t, 16> multipliedDamageRolls;
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