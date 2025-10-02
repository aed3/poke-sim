#pragma once

#include <Components/Damage.hpp>
#include <Config/Require.hpp>
#include <Types/Damage.hpp>
#include <Types/Entity.hpp>
#include <Types/Event.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Move.hpp>
#include <Types/Random.hpp>
#include <Utilities/MaxSizedVector.hpp>

namespace pokesim {
namespace simulate_turn {
struct TurnOutcomeBattles {
  types::entityVector val{};
};
}  // namespace simulate_turn

namespace calc_damage {
struct UsesUntilKo {
 private:
  struct Uses {
    types::moveHits hits = 0U;
    types::damageRollIndex damageRollsIncluded = 0U;

    bool operator==(const Uses& other) const {
      return hits == other.hits && damageRollsIncluded == other.damageRollsIncluded;
    }
  };

 public:
  internal::maxSizedVector<Uses, MechanicConstants::DamageRollCount::MAX> val{};

  const Uses& minUses() const {
    POKESIM_REQUIRE(!val.empty(), "UsesUntilKo has no values to read.");
    return val.front();
  }

  const Uses& maxUses() const {
    POKESIM_REQUIRE(!val.empty(), "UsesUntilKo has no values to read.");
    return val.back();
  }

  bool guaranteedKo() const {
    const Uses& min = minUses();
    return min.hits == 1U && min.damageRollsIncluded == MechanicConstants::DamageRollCount::MAX;
  }
};

struct AttackerHpRecovered : DamageRolls {};
struct AttackerHpLost : DamageRolls {};
}  // namespace calc_damage

namespace analyze_effect {
struct EffectMultiplier {
  types::effectMultiplier val = MechanicConstants::AnalyzeEffectMultiplier::BASE;
};

using MultipliedDamageRolls = DamageRolls;
using MultipliedUsesUntilKo = calc_damage::UsesUntilKo;

namespace tags {
struct InfiniteMultiplier {};
}  // namespace tags
}  // namespace analyze_effect
}  // namespace pokesim