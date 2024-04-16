#pragma once

#include <Utilities/MaxSizedVector.hpp>
#include <cstdint>
#include <type_traits>

#include "Entity.hpp"

namespace pokesim::types {
namespace internal {
const std::uint8_t MAX_TEAM_SIZE = 6U;
const std::uint8_t MAX_ACTIVE_POKEMON_SLOTS = 2U;
const std::uint8_t MAX_MOVE_SLOTS = 4U;
}  // namespace internal

using stateId = std::underlying_type_t<entity>;
using stateProbability = float;
using stateRngSeed = std::uint32_t;

using battleTurn = std::uint16_t;

using cloneIndex = std::underlying_type_t<entity>;

using teamPositionIndex = std::uint8_t;
using moveSlotPosition = std::uint8_t;

template <typename T>
using teamPositions = pokesim::internal::maxSizedVector<T, internal::MAX_TEAM_SIZE>;
using teamOrder = types::teamPositions<types::teamPositionIndex>;

template <typename T>
using moveSlots = pokesim::internal::maxSizedVector<T, internal::MAX_MOVE_SLOTS>;

template <typename T>
using sideSlots = pokesim::internal::maxSizedVector<T, internal::MAX_ACTIVE_POKEMON_SLOTS>;
}  // namespace pokesim::types