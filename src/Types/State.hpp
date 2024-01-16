#pragma once

#include <cstdint>
#include <vector>

namespace pokesim::types {
using stateId = std::uint16_t;
using stateProbability = float;
using stateRngSeed = std::uint32_t;

using battleTurn = std::uint16_t;

using teamPositionIndex = std::uint8_t;
using moveSlotPosition = std::uint8_t;

template <typename T>
using teamPositions = std::vector<T>;

template <typename T>
using sideSlots = std::vector<T>;
}  // namespace pokesim::types