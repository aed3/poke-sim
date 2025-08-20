#pragma once

#include <Components/EntityHolders/Current.hpp>
#include <Components/Tags/Current.hpp>

namespace pokesim::calc_damage {
using Attacker = CurrentActionSource;
using Defenders = CurrentActionTargets;
using UsedMovesAsAttacker = CurrentActionMovesAsSource;
using UsedMovesAsDefender = CurrentActionMovesAsTarget;

namespace tags {
using Attacker = pokesim::tags::CurrentActionMoveSource;
using Defender = pokesim::tags::CurrentActionMoveTarget;
using UsedMove = pokesim::tags::CurrentActionMove;
}  // namespace tags
}  // namespace pokesim::calc_damage