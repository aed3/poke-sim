#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Random.hpp>
#include <Types/Stats.hpp>
#include <array>

namespace pokesim::calc_damage {
struct CritChanceDivisor {
  types::percentChance val = pokesim::MechanicConstants::CRIT_CHANCE_DIVISORS[0];
};

struct CritBoost {
  types::boost val = 0U;
};

namespace tags {
struct Crit {};
}  // namespace tags
}  // namespace pokesim::calc_damage