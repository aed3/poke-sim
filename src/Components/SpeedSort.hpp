#pragma once

#include <cstdint>

namespace pokesim {
// Data that determines the order actions take place
struct SpeedSort {
  // Order of the types of actions (lower first)
  std::uint8_t order = 0;
  // Priority of the action (lower first)
  std::int8_t priority = 0;
  // Fractional priority of the action (lower first)
  std::int8_t fractionalPriority = 0;
  // Speed of Pokemon using move (higher first if priority tie)
  std::uint16_t speed = 1;
};
}  // namespace pokesim