#pragma once

#include <Types/Constants.hpp>
#include <Types/State.hpp>
#include <Utilities/FixedMemoryVector.hpp>

namespace pokesim {
struct SpeedTieIndexes {
  struct Span {
    types::activePokemonIndex start = Constants::ActivePokemon::MIN;
    types::activePokemonIndex length = Constants::ActivePokemon::MIN;

    bool operator==(const Span& other) const noexcept { return other.start == start && other.length == length; }
  };

  internal::fixedMemoryVector<Span, Constants::ActivePokemon::MAX> val{};
};
}  // namespace pokesim
