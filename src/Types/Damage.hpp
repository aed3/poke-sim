#pragma once

#include <array>
#include <cstdint>

namespace pokesim::types {
namespace internal {
const std::uint8_t MAX_DAMAGE_ROLL_COUNT = 16U;
}

using damage = std::uint16_t;
using damageRolls = std::array<damage, internal::MAX_DAMAGE_ROLL_COUNT>;
}  // namespace pokesim::types