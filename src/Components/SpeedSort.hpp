#pragma once

#include <cstdint>

namespace pokesim {
struct SpeedSort {
  std::uint8_t order = 0;
  std::int8_t priority = 0;
  std::int8_t fractionalPriority = 0;
  std::uint16_t speed = 1;
};
}  // namespace pokesim