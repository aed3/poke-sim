#pragma once

#include <array>
#include <cstdint>

namespace pokesim::types {
namespace internal {
const std::uint8_t MAX_DAMAGE_ROLL_COUNT = 16U;
}

using Damage = std::uint16_t;
using DamageRolls = std::array<Damage, internal::MAX_DAMAGE_ROLL_COUNT>;
}  // namespace pokesim::types