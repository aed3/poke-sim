#pragma once

#include <Types/Enums/ActionOrder.hpp>
#include <Types/Move.hpp>
#include <Types/State.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
// Data that determines the order actions take place
struct SpeedSort {
  // Order of the types of actions (lower first)
  ActionOrder order = ActionOrder::NONE;
  // Priority of the action (higher first)
  types::priority priority = 0;
  // Fractional priority of the action (higher first)
  types::fractionalPriority fractionalPriority = 0;
  // Speed of Pokemon using move (higher first if priority tie)
  types::stat speed = 1;
};
}  // namespace pokesim