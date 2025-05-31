#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/State.hpp>
#include <Utilities/FixedMemoryVector.hpp>
#include <cstdint>

namespace pokesim {
struct SpeedTieIndexes {
  struct Span {
    types::activePokemonIndex start = 0;
    types::activePokemonIndex length = 0;
  };

  internal::fixedMemoryVector<Span, MechanicConstants::MAX_ACTIVE_POKEMON> val{};
};
}  // namespace pokesim