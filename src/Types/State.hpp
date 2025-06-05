#pragma once

#include <Utilities/FixedMemoryVector.hpp>
#include <optional>
#include <type_traits>

#include "Entity.hpp"
#include "MechanicConstants.hpp"
#include "NumberToType.hpp"

namespace pokesim {
class Simulation;

namespace types {
using stateId = std::underlying_type_t<entity>;

using battleTurn = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::TURN_COUNT>;

using cloneIndex = std::underlying_type_t<entity>;

using teamPositionIndex = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::TEAM_SIZE>;
using moveSlotIndex = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::MOVE_SLOTS>;
using activePokemonIndex = pokesim::internal::unsignedIntType<MechanicConstants::MaxValues::ACTIVE_POKEMON>;

template <typename T>
using teamPositions = pokesim::internal::fixedMemoryVector<T, MechanicConstants::MaxValues::TEAM_SIZE>;
using teamOrder = types::teamPositions<types::teamPositionIndex>;

template <typename T>
using moveSlots = pokesim::internal::fixedMemoryVector<T, MechanicConstants::MaxValues::MOVE_SLOTS>;

template <typename T>
using sideSlots = pokesim::internal::fixedMemoryVector<T, MechanicConstants::MaxValues::ACTIVE_POKEMON_SLOTS_PER_SIDE>;

template <typename T>
using targets = pokesim::internal::fixedMemoryVector<T, MechanicConstants::MaxValues::TARGETS>;

using callback = void (*)(Simulation&);
using optionalCallback = std::optional<void (*)(Simulation&)>;
}  // namespace types
}  // namespace pokesim