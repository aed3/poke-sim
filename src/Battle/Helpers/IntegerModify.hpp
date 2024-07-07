#pragma once

#include <Types/MechanicConstants.hpp>
#include <cstdint>

namespace pokesim {
template <typename Number1, typename Number2>
std::uint32_t fixedPointMultiply(Number1 value, Number2 multiplier) {
  std::uint32_t modifier = multiplier * MechanicConstants::FIXED_POINT_DIVISOR;
  std::uint32_t modified = value * modifier;
  std::uint32_t result = (value + MechanicConstants::FIXED_POINT_INTEGER_BITS) / MechanicConstants::FIXED_POINT_DIVISOR;
  return result;
}
}  // namespace pokesim