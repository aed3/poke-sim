#pragma once

#include <Config/Require.hpp>
#include <Types/Damage.hpp>
#include <Types/Enums/StabBoostKind.hpp>
#include <Types/Event.hpp>
#include <Types/MechanicConstants.hpp>
#include <Utilities/MaxSizedVector.hpp>
#include <vector>

namespace pokesim {
struct Damage {
  types::damage val = MechanicConstants::Damage::MIN;
};

struct DamageRollModifiers {
  StabBoostKind stab = StabBoostKind::NONE;
  types::typeEffectiveness typeEffectiveness = MechanicConstants::TypeEffectivenessShift::BASE;
  bool burn = false;
  bool zOrMaxBrokenProtect = false;
  types::eventModifier modifyDamageEvent = MechanicConstants::FIXED_POINT_SCALE;

  bool operator==(const DamageRollModifiers& other) const {
    return stab == other.stab && typeEffectiveness == other.typeEffectiveness && burn == other.burn &&
           modifyDamageEvent == other.modifyDamageEvent && zOrMaxBrokenProtect == other.zOrMaxBrokenProtect;
  }
};

struct DamageRolls {
  internal::maxSizedVector<Damage, MechanicConstants::DamageRollCount::MAX> val{};

  DamageRolls() {}
  DamageRolls(const DamageRolls& other) : val(other.val) {}

  DamageRolls(const std::vector<types::damage>& list) {
    POKESIM_REQUIRE(
      list.size() <= MechanicConstants::DamageRollCount::MAX,
      "More damage rolls are being added than allowed.");
    val.reserve((types::damageRollIndex)list.size());
    for (types::damage damage : list) {
      val.push_back({damage});
    }
  }

  types::damage min() const {
    POKESIM_REQUIRE(!val.empty(), "DamageRolls has no values to read.");
    return val.back().val;
  }

  types::damage max() const {
    POKESIM_REQUIRE(!val.empty(), "DamageRolls has no values to read.");
    return val.front().val;
  }
};
}  // namespace pokesim