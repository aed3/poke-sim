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
  struct KoProbability {
    types::moveHits uses = 0U;
    types::probability probability = MechanicConstants::Probability::MIN;

    bool operator==(const KoProbability& other) const { return uses == other.uses && probability == other.probability; }
  };

 public:
  internal::maxSizedVector<KoProbability, MechanicConstants::DamageRollCount::MAX> val{};

  const KoProbability& minHits() const {
    POKESIM_REQUIRE(!val.empty(), "UsesUntilKo has no values to read.");
    return val.front();
  }

  const KoProbability& maxHits() const {
    POKESIM_REQUIRE(!val.empty(), "UsesUntilKo has no values to read.");
    return val.back();
  }

  bool guaranteedKo() const {
    const KoProbability& min = minHits();
    return min.uses == 1U && min.probability == MechanicConstants::Probability::MAX;
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