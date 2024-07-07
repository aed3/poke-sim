#pragma once

#include <Types/Stats.hpp>

namespace pokesim::calc_damage {
struct AttackingLevel {
  types::level val = 1;
};

struct AttackingStat {
  types::stat val = 1;
};

struct DefendingStat {
  types::stat val = 1;
};

namespace tags {
struct P1Defending {};
struct P2Defending {};
}  // namespace tags
}  // namespace pokesim::calc_damage