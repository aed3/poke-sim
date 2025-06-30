#pragma once

#include <Types/Damage.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Random.hpp>
#include <Types/Stats.hpp>
#include <array>

namespace pokesim::calc_damage {
struct CritChanceDivisor {
  types::percentChance val = pokesim::MechanicConstants::CRIT_CHANCE_DIVISORS[0];
};

struct CritBoost {
  types::critBoost val = pokesim::MechanicConstants::CritBoost::MIN;
};

namespace tags {
struct Crit {};
}  // namespace tags
}  // namespace pokesim::calc_damage