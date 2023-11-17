#pragma once

#include <cstdint>

namespace pokesim::types {
using StateId = std::uint16_t;
using StateProbability = float;
using StateRngSeed = std::uint32_t;

using BattleTurn = std::uint16_t;

using ActionOrder = std::uint8_t;

using TeamSlotPosition = std::uint8_t;
}  // namespace pokesim::types