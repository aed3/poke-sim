#pragma once

#include <array>
#include <cstdint>

namespace pokesim {
// Denotes if the simulated battle is a single or double battle
enum class FractionalPriority : std::int8_t {
  NONE = 0,
  PLUS = 1,
  MINUS = -1,
};

namespace internal {
static constexpr inline std::array<FractionalPriority, 3U> VALID_FRACTIONAL_PRIORITIES = {
  FractionalPriority::NONE,
  FractionalPriority::PLUS,
  FractionalPriority::MINUS,
};
}  // namespace internal
}  // namespace pokesim
