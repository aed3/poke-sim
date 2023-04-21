#pragma once

#include <cstdint>

namespace pokesim {
struct SpeedSort {
  std::uint8_t order;
  std::int8_t priority;
  std::int8_t fractionalPriority;
  std::uint16_t speed;
};
}  // namespace pokesim