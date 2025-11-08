#pragma once

#include <Components/Damage.hpp>
#include <Config/Require.hpp>
#include <Types/Enums/DamageRollKind.hpp>
#include <Types/MechanicConstants.hpp>
#include <type_traits>

namespace pokesim::calc_damage {
inline types::damage averageOfDamageRolls(const DamageRolls& damageRolls, DamageRollKind damageRollKind) {
  POKESIM_REQUIRE(!damageRolls.val.empty(), "DamageRolls has no rolls yet.");

  if (damageRollKind & DamageRollKind::ALL_DAMAGE_ROLLS) {
    POKESIM_REQUIRE(
      damageRolls.val.size() == MechanicConstants::DamageRollCount::MAX,
      "DamageRolls does not have all rolls yet.");
    return damageRolls.val[MechanicConstants::DamageRollCount::MAX / 2].val;
  }
  POKESIM_REQUIRE(damageRollKind & DamageRollKind::AVERAGE_DAMAGE, "DamageRolls does not contain average");

  if (damageRollKind & DamageRollKind::MAX_DAMAGE) {
    POKESIM_REQUIRE(damageRolls.val.size() > 1U, "DamageRolls may not have average roll yet.");
    return damageRolls.val[1].val;
  }
  return damageRolls.val[0].val;
}
}  // namespace pokesim::calc_damage