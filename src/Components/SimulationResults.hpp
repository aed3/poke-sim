#pragma once

#include <Components/Damage.hpp>
#include <Config/Require.hpp>
#include <Types/Damage.hpp>
#include <Types/Entity.hpp>
#include <Types/Move.hpp>
#include <vector>

namespace pokesim {
namespace simulate_turn {
struct TurnOutcomeBattles {
  std::vector<types::entity> val{};
};
}  // namespace simulate_turn

namespace calc_damage {
struct UsesUntilKo {
 private:
  struct KoChance {
    types::moveHits uses = 0;
    types::useUntilKoChance chance = 0.0F;

    bool operator==(const KoChance& other) const { return uses == other.uses && chance == other.chance; }
  };

 public:
  std::vector<KoChance> val{};

  const KoChance& minHits() const {
    POKESIM_REQUIRE(!val.empty(), "UsesUntilKo has no values to read.");
    return val.front();
  }

  const KoChance& maxHits() const {
    POKESIM_REQUIRE(!val.empty(), "UsesUntilKo has no values to read.");
    return val.back();
  }

  bool guaranteedKo() const {
    const KoChance& min = minHits();
    return min.uses == 1 && min.chance == 1.0F;
  }
};

struct AttackerHpRecovered : DamageRolls {};
struct AttackerHpLost : DamageRolls {};
}  // namespace calc_damage

namespace analyze_effect {
struct EffectMultiplier {
  types::effectMultiplier val = 1.0F;
};

using MultipliedDamageRolls = DamageRolls;
using MultipliedUsesUntilKo = calc_damage::UsesUntilKo;

namespace tags {
struct InfiniteMultiplier {};
}  // namespace tags
}  // namespace analyze_effect
}  // namespace pokesim