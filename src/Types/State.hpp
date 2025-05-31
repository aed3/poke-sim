#pragma once

#include <Utilities/FixedMemoryVector.hpp>
#include <cstdint>
#include <optional>
#include <type_traits>

#include "Entity.hpp"
#include "MechanicConstants.hpp"

namespace pokesim {
class Simulation;

namespace types {
using stateId = std::underlying_type_t<entity>;

using battleTurn = std::uint16_t;

using cloneIndex = std::underlying_type_t<entity>;

using teamPositionIndex = std::uint8_t;
using moveSlotPosition = std::uint8_t;
using activePokemonIndex = std::uint8_t;

template <typename T>
using teamPositions = pokesim::internal::fixedMemoryVector<T, MechanicConstants::MAX_TEAM_SIZE>;
using teamOrder = types::teamPositions<types::teamPositionIndex>;

template <typename T>
using moveSlots = pokesim::internal::fixedMemoryVector<T, MechanicConstants::MAX_MOVE_SLOTS>;

template <typename T>
using sideSlots = pokesim::internal::fixedMemoryVector<T, MechanicConstants::MAX_ACTIVE_POKEMON_SLOTS_PER_SIDE>;

template <typename T>
using targets = pokesim::internal::fixedMemoryVector<T, MechanicConstants::MAX_TARGETS>;

using callback = void (*)(Simulation&);
using optionalCallback = std::optional<void (*)(Simulation&)>;
}  // namespace types
}  // namespace pokesim