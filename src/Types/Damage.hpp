#pragma once

#include <array>
#include <cstdint>

#include "MechanicConstants.hpp"

namespace pokesim::types {
using damage = std::uint16_t;
using damageRoll = std::uint8_t;
using allDamageRolls = std::array<damage, pokesim::MechanicConstants::MAX_DAMAGE_ROLL_COUNT>;
}  // namespace pokesim::types