#pragma once

#include <Types/Entity.hpp>
#include <Types/State.hpp>

namespace pokesim::calc_damage {
struct CalcDamageTargets {
  types::targets<types::entity> val{};
};

namespace tags {
struct CalcDamageTarget {};
}  // namespace tags
}  // namespace pokesim::calc_damage