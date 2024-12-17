#pragma once

#include <Types/Damage.hpp>
#include <Types/Event.hpp>
#include <Types/MechanicConstants.hpp>
#include <vector>

namespace pokesim {
struct Damage {
  types::damage val = 1;
};

struct DamageRollModifier {
  types::eventModifier val = MechanicConstants::FIXED_POINT_SCALE;
};

struct DamageRolls {
  std::vector<Damage> val{};

  DamageRolls(std::initializer_list<types::damage> list) {
    val.reserve(list.size());
    for (types::damage damage : list) {
      val.push_back({damage});
    }
  }
};
}  // namespace pokesim