#pragma once

#include <Components/EntityHolders/Current.hpp>
#include <Components/Tags/Current.hpp>

namespace pokesim::calc_damage {
using Attacker = CurrentActionSource;
using Defenders = CurrentActionTargets;
using Move = CurrentActionMove;

namespace tags {
using Attacker = pokesim::tags::CurrentActionMoveSource;
using Defender = pokesim::tags::CurrentActionMoveTarget;
using Move = pokesim::tags::CurrentActionMove;
}  // namespace tags
}  // namespace pokesim::calc_damage