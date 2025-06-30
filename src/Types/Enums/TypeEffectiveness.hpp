#pragma once

#include <array>
#include <cstdint>

namespace pokesim {
enum class TypeEffectiveness : std::uint8_t {
  IMMUNE,
  NOT_VERY_EFFECTIVE,
  NEUTRAL,
  SUPER_EFFECTIVE,
};

static constexpr inline std::array<TypeEffectiveness, 4U> VALID_TYPE_EFFECTIVENESS = {
  TypeEffectiveness::IMMUNE,
  TypeEffectiveness::NOT_VERY_EFFECTIVE,
  TypeEffectiveness::NEUTRAL,
  TypeEffectiveness::SUPER_EFFECTIVE,
};
}  // namespace pokesim
