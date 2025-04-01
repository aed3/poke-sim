#pragma once

#include <entt/config/config.h>

#include <Components/Damage.hpp>
#include <Types/Enums/DamageRollKind.hpp>
#include <Types/MechanicConstants.hpp>
#include <type_traits>

namespace pokesim::calc_damage {
inline constexpr bool damageKindsMatch(DamageRollKind kindA, DamageRollKind kindB) {
  using DamageRollKindBase = std::underlying_type_t<DamageRollKind>;
  return ((DamageRollKindBase)kindA & (DamageRollKindBase)kindB) != 0;
}

inline types::damage averageOfDamageRolls(const DamageRolls& damageRolls, DamageRollKind damageRollKind) {
  ENTT_ASSERT(!damageRolls.val.empty(), "DamageRolls has no rolls yet.");

  if (damageKindsMatch(damageRollKind, DamageRollKind::ALL_DAMAGE_ROLLS)) {
    ENTT_ASSERT(
      damageRolls.val.size() == MechanicConstants::MAX_DAMAGE_ROLL_COUNT,
      "DamageRolls does not have all rolls yet.");
    return damageRolls.val[MechanicConstants::MAX_DAMAGE_ROLL_COUNT / 2].val;
  }
  ENTT_ASSERT(damageKindsMatch(damageRollKind, DamageRollKind::AVERAGE_DAMAGE), "DamageRolls does not contain average");

  if (damageKindsMatch(damageRollKind, DamageRollKind::MAX_DAMAGE)) {
    ENTT_ASSERT(damageRolls.val.size() > 1, "DamageRolls may not have average roll yet.");
    return damageRolls.val[1].val;
  }
  return damageRolls.val[0].val;
}

template <
  typename... CombinedKinds,
  typename = std::enable_if_t<std::conjunction_v<std::is_same<CombinedKinds, DamageRollKind>...>>>
inline constexpr DamageRollKind combineDamageKinds(CombinedKinds... kinds) {
  using DamageRollKindBase = std::underlying_type_t<DamageRollKind>;
  return static_cast<DamageRollKind>((static_cast<DamageRollKindBase>(kinds) | ...));
}
}  // namespace pokesim::calc_damage