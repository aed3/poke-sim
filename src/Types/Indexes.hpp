
#pragma once

#include <Utilities/NumberToType.hpp>

#include "Constants.hpp"

namespace pokesim::types {
using battleTurn = pokesim::internal::unsignedIntType<Constants::TurnCount::MAX>;
using actionQueueIndex = pokesim::internal::unsignedIntType<Constants::ActionQueueLength::MAX>;
using sideIndex = pokesim::internal::unsignedIntType<Constants::SIDE_COUNT>;
using teamPositionIndex = pokesim::internal::unsignedIntType<Constants::TeamSize::MAX>;
using activePokemonIndex = pokesim::internal::unsignedIntType<Constants::ActivePokemon::MAX>;
using moveSlotIndex = pokesim::internal::unsignedIntType<Constants::MoveSlots::MAX>;
using speciesTypeIndex = pokesim::internal::unsignedIntType<Constants::TYPES_PER_POKEMON>;
}  // namespace pokesim::types
