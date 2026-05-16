#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/State.hpp>
#include <Utilities/FixedMemoryVector.hpp>

namespace pokesim {
struct SpeedTieIndexes {
  struct Span {
    types::activePokemonIndex start = MechanicConstants::ActivePokemon::MIN;
    types::activePokemonIndex length = MechanicConstants::ActivePokemon::MIN;

    bool operator==(const Span& other) const noexcept { return other.start == start && other.length == length; }
  };

  internal::fixedMemoryVector<Span, MechanicConstants::ActivePokemon::MAX> val{};
};
}  // namespace pokesim
