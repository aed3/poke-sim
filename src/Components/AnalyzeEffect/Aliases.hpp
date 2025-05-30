#pragma once

#include <Components/EntityHolders/Current.hpp>
#include <Components/Tags/Current.hpp>

namespace pokesim::analyze_effect {
using Attacker = CurrentActionSource;
using Defenders = CurrentActionTargets;

namespace tags {
using Attacker = pokesim::tags::CurrentActionMoveSource;
using Defender = pokesim::tags::CurrentActionMoveTarget;
using Move = pokesim::tags::CurrentActionMove;
}  // namespace tags
}  // namespace pokesim::analyze_effect