#pragma once

#include <Types/Constants.hpp>
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
  types::priority priority = Constants::MovePriority::DEFAULT;
  // Whether negative fractional priority is active for the action (false first)
  types::fractionalPriority fractionalPriority = false;
  // Speed of Pokemon using move (higher first if priority tie)
  types::stat speed = Constants::PokemonEffectiveStat::DEFAULT;

  bool operator==(const SpeedSort& other) const {
    return order == other.order && priority == other.priority && fractionalPriority == other.fractionalPriority &&
           speed == other.speed;
  }
};
}  // namespace pokesim
