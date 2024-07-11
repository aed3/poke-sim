#pragma once

#include <cstdint>

namespace pokesim {
enum class TypeEffectiveness : std::uint8_t {
  IMMUNE,
  NOT_VERY_EFFECTIVE,
  NEUTRAL,
  SUPER_EFFECTIVE,
};
}  // namespace pokesim
