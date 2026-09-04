#pragma once

#include <Components/EntityHolders/Current.hpp>
#include <Components/Tags/Current.hpp>

namespace pokesim::calc_damage {
using Attacker = CurrentActionSource;
using Defender = CurrentActionTarget;

namespace tags {
using Attacker = pokesim::tags::CurrentActionSource;
using Defender = pokesim::tags::CurrentActionTarget;
using UsedMove = pokesim::tags::CurrentActionMove;
using FailedUsedMove = pokesim::tags::FailedCurrentActionMove;
}  // namespace tags
}  // namespace pokesim::calc_damage
