#pragma once

#include <Components/Damage.hpp>
#include <Config/Require.hpp>
#include <Types/Constants.hpp>
#include <Types/Damage.hpp>
#include <Types/Entity.hpp>
#include <Types/Event.hpp>
#include <Types/FixedMemoryVector.hpp>
#include <Types/Move.hpp>
#include <Types/Random.hpp>

namespace pokesim {
namespace simulate_turn::tags {
struct BattleOutcome {};
}  // namespace simulate_turn::tags

namespace calc_damage {
struct UsesUntilKo {
  struct Uses {
    types::moveUses uses = 0U;
    types::damageRollIndex damageRollsIncluded = 0U;

    constexpr bool operator==(const Uses& other) const {
      return uses == other.uses && damageRollsIncluded == other.damageRollsIncluded;
    }
  };

  types::fixedMemoryVector<Uses, Constants::DamageRollCount::MAX> val{};

  constexpr const Uses& minUses() const { return val.front(); }
  constexpr const Uses& maxUses() const { return val.back(); }

  constexpr bool guaranteedKo() const {
    const Uses& min = minUses();
    return min.uses == 1U && min.damageRollsIncluded == Constants::DamageRollCount::MAX;
  }
};

struct AttackerHpRecovered : DamageRolls {};
struct AttackerHpLost : DamageRolls {};

namespace tags {
struct DefenderImmune {};
struct IgnoredStatusMove {};
}  // namespace tags
}  // namespace calc_damage

namespace analyze_effect {
struct EffectMultiplier {
  types::effectMultiplier val = Constants::AnalyzeEffectMultiplier::DEFAULT;
};

using MultipliedDamageRolls = DamageRolls;
using MultipliedUsesUntilKo = calc_damage::UsesUntilKo;

namespace tags {
struct InfiniteMultiplier {};
struct IgnoredInput {};
}  // namespace tags
}  // namespace analyze_effect
}  // namespace pokesim
