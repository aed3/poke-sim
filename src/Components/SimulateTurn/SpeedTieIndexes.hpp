#pragma once

#include <Types/Constants.hpp>
#include <Types/FixedMemoryVector.hpp>
#include <Types/Indexes.hpp>

namespace pokesim {
struct SpeedTieIndexes {
  struct Span {
    types::activePokemonIndex start = Constants::ActivePokemon::MIN;
    types::activePokemonIndex length = Constants::ActivePokemon::MIN;

    bool operator==(const Span& other) const noexcept { return other.start == start && other.length == length; }
  };

  types::fixedMemoryVector<Span, Constants::ActivePokemon::MAX> val{};
};
}  // namespace pokesim
