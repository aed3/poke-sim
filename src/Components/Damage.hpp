#pragma once

#include <Config/Require.hpp>
#include <Types/Constants.hpp>
#include <Types/Damage.hpp>
#include <Types/FixedMemoryVector.hpp>
#include <vector>

namespace pokesim {
struct Damage {
  types::damage val = Constants::Damage::DEFAULT;
};

struct DamageRolls {
  types::fixedMemoryVector<Damage, Constants::DamageRollCount::MAX> val{};

  DamageRolls() {}
  DamageRolls(const DamageRolls& other) : val(other.val) {}
  DamageRolls(const std::vector<types::damage>& list) {
    POKESIM_REQUIRE(list.size() <= Constants::DamageRollCount::MAX, "More damage rolls are being added than allowed.");
    for (types::damage damage : list) {
      val.push_back({damage});
    }
  }

  constexpr types::damage min() const { return val.back().val; }
  constexpr types::damage max() const { return val.front().val; }
};
}  // namespace pokesim
