#pragma once

#include <Types/Move.hpp>
#include <Types/State.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
// Data that determines the order actions take place
struct SpeedSort {
  // Order of the types of actions (lower first)
  types::ActionOrder order = 0;
  // Priority of the action (lower first)
  types::Priority priority = 0;
  // Fractional priority of the action (lower first)
  types::FractionalPriority fractionalPriority = 0;
  // Speed of Pokemon using move (higher first if priority tie)
  types::Stat speed = 1;
};
}  // namespace pokesim