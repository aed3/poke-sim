#pragma once

#include <Components/EntityHolders/Current.hpp>
#include <Components/Tags/Current.hpp>

namespace pokesim::analyze_effect {
using Attacker = CurrentActionSource;
using Defender = CurrentActionTarget;

namespace tags {
using Attacker = pokesim::tags::CurrentActionSource;
using Defender = pokesim::tags::CurrentActionTarget;
using Move = pokesim::tags::CurrentActionMove;
}  // namespace tags
}  // namespace pokesim::analyze_effect
