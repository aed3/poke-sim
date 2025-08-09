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

using battleTurn = pokesim::internal::unsignedIntType<MechanicConstants::TurnCount::MAX>;

using entityIndex = std::underlying_type_t<entity>;

using teamPositionIndex = pokesim::internal::unsignedIntType<MechanicConstants::TeamSize::MAX>;
using moveSlotIndex = pokesim::internal::unsignedIntType<MechanicConstants::MoveSlots::MAX>;
using activePokemonIndex = pokesim::internal::unsignedIntType<MechanicConstants::ActivePokemon::MAX>;

template <typename T>
using teamPositions = pokesim::internal::fixedMemoryVector<T, MechanicConstants::TeamSize::MAX>;
using teamOrder = types::teamPositions<types::teamPositionIndex>;

template <typename T>
using moveSlots = pokesim::internal::fixedMemoryVector<T, MechanicConstants::MoveSlots::MAX>;

template <typename T>
using sideSlots = pokesim::internal::fixedMemoryVector<T, MechanicConstants::ActivePokemonSlotsPerSide::MAX>;

template <typename T>
using targets = pokesim::internal::fixedMemoryVector<T, MechanicConstants::Targets::MAX>;

using callback = void (*)(Simulation&);
using optionalCallback = std::optional<void (*)(Simulation&)>;
}  // namespace types
}  // namespace pokesim