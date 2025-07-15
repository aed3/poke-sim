#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/State.hpp>

namespace pokesim {
// The current turn of a battle
struct Turn {
  types::battleTurn val = MechanicConstants::TurnCount::MIN;
};
}  // namespace pokesim