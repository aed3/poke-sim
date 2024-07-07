#pragma once

#include <Types/Enums/DamageRollKind.hpp>
#include <type_traits>

namespace pokesim::calc_damage {
inline bool damageKindsMatch(DamageRollKind kindA, DamageRollKind kindB) {
  using DamageRollKindBase = std::underlying_type_t<DamageRollKind>;
  return ((DamageRollKindBase)kindA & (DamageRollKindBase)kindB) != 0;
}

template <typename... CombinedKinds>
inline void setDamageKinds(DamageRollKind& kind, CombinedKinds... kinds) {
  using DamageRollKindBase = std::underlying_type_t<DamageRollKind>;
  kind = static_cast<DamageRollKind>((static_cast<DamageRollKindBase>(kinds) | ...));
}
}  // namespace pokesim::calc_damage