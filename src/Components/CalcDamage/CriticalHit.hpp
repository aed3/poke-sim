#pragma once

#include <Types/Constants.hpp>
#include <Types/Damage.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Random.hpp>
#include <Types/Stats.hpp>

namespace pokesim::calc_damage {
struct CritChanceDivisor {
  types::percentChance val = 1U;
};

struct CritBoost {
  types::critBoost val = pokesim::Constants::CritBoost::DEFAULT;
};

namespace tags {
struct Crit {};
}  // namespace tags
}  // namespace pokesim::calc_damage
