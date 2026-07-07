#pragma once

#include <Types/Constants.hpp>
#include <Types/Decisions.hpp>
#include <Types/Enums/ActionOrder.hpp>
#include <Types/Move.hpp>
#include <Types/Random.hpp>
#include <Types/State.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
struct ActionQueueItem {
  // Order of the types of actions (lower first).
  ActionOrder order = ActionOrder::NONE;
  // Priority of the action (higher first).
  types::priority priority = Constants::MovePriority::DEFAULT;
  // Whether negative fractional priority is active for the action (false first).
  types::fractionalPriority fractionalPriority = false;
  // Speed of Pokemon using move (higher first if priority tie).
  types::stat speed = Constants::PokemonEffectiveStat::DEFAULT;

  // The decision that created this action queue item.
  types::slotDecision decision{};

  constexpr bool operator==(const ActionQueueItem& other) const {
    return isSameSpeed(other) && decision == other.decision;
  }

  constexpr bool isSameSpeed(const ActionQueueItem& other) const {
    return order == other.order && priority == other.priority && fractionalPriority == other.fractionalPriority &&
           speed == other.speed;
  }

  constexpr bool isFasterThan(const ActionQueueItem& other) const {
    if (order != other.order) {
      return order < other.order;
    }

    if (priority != other.priority) {
      return priority > other.priority;
    }

    if (fractionalPriority != other.fractionalPriority) {
      return !fractionalPriority;
    }

    if (speed != other.speed) {
      return speed > other.speed;
    }

    return false;
  }
};

struct ActionQueue {
  types::maxSizedVector<ActionQueueItem, Constants::ActionQueueLength::MAX> val{};
};
}  // namespace pokesim
