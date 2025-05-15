#pragma once

#include <Config/Require.hpp>
#include <Types/Damage.hpp>
#include <Types/Enums/StabBoostKind.hpp>
#include <Types/Event.hpp>
#include <Types/MechanicConstants.hpp>
#include <vector>

namespace pokesim {
struct Damage {
  types::damage val = 1;
};

struct DamageRollModifiers {
  StabBoostKind stab = StabBoostKind::NONE;
  types::typeEffectiveness typeEffectiveness = 0;
  bool burn = false;
  bool zOrMaxBrokenProtect = false;
  types::eventModifier modifyDamageEvent = MechanicConstants::FIXED_POINT_SCALE;

  bool operator==(const DamageRollModifiers& other) const {
    return stab == other.stab && typeEffectiveness == other.typeEffectiveness && burn == other.burn &&
           modifyDamageEvent == other.modifyDamageEvent && zOrMaxBrokenProtect == other.zOrMaxBrokenProtect;
  }
};

struct DamageRolls {
  std::vector<Damage> val{};

  DamageRolls(std::initializer_list<types::damage> list) {
    val.reserve(list.size());
    for (types::damage damage : list) {
      val.push_back({damage});
    }
  }

  types::damage min() const {
    POKESIM_ASSERT(!val.empty(), "DamageRolls has no values to read.");
    return val.back().val;
  }

  types::damage max() const {
    POKESIM_ASSERT(!val.empty(), "DamageRolls has no values to read.");
    return val.front().val;
  }
};
}  // namespace pokesim