#pragma once

#include <cstdint>
#include <type_traits>
#include <vector>

#include "Entity.hpp"

namespace pokesim::types {
using stateId = std::underlying_type_t<entity>;
using stateProbability = float;
using stateRngSeed = std::uint32_t;

using battleTurn = std::uint16_t;

using cloneIndex = std::underlying_type_t<entity>;

using teamPositionIndex = std::uint8_t;
using moveSlotPosition = std::uint8_t;

template <typename T>
using teamPositions = std::vector<T>;

template <typename T>
using sideSlots = std::vector<T>;
}  // namespace pokesim::types