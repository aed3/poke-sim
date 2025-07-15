#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/State.hpp>
#include <Utilities/FixedMemoryVector.hpp>
#include <cstdint>

namespace pokesim {
struct SpeedTieIndexes {
  struct Span {
    types::activePokemonIndex start = MechanicConstants::ActivePokemon::MIN;
    types::activePokemonIndex length = MechanicConstants::ActivePokemon::MIN;
  };

  internal::fixedMemoryVector<Span, MechanicConstants::ActivePokemon::MAX> val{};
};
}  // namespace pokesim