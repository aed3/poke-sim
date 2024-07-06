#pragma once

#include <Types/Random.hpp>
#include <Types/Stats.hpp>
#include <array>

namespace pokesim::calc_damage {
namespace internal {
static const std::array<types::percentChance, 4U> CRIT_CHANCE_DIVISORS{24U, 8U, 2U, 1U};
}

struct CritChanceDivisor {
  types::percentChance val = internal::CRIT_CHANCE_DIVISORS[0];
};

struct CritBoost {
  types::boost val = 0U;
};

namespace tags {
struct Crit {};
}
}  // namespace pokesim::calc_damage