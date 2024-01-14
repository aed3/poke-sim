#pragma once

#include <cstdint>
#include <vector>

namespace pokesim::types {
using StateId = std::uint16_t;
using StateProbability = float;
using StateRngSeed = std::uint32_t;

using BattleTurn = std::uint16_t;

using TeamPositionIndex = std::uint8_t;
using MoveSlotPosition = std::uint8_t;

template <typename T>
using TeamPositions = std::vector<T>;

template <typename T>
using SideSlots = std::vector<T>;
}  // namespace pokesim::types