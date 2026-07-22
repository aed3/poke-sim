#pragma once

#include <Types/Constants.hpp>
#include <Types/Indexes.hpp>

namespace pokesim {
// The current turn of a battle
struct Turn {
  types::battleTurn val = Constants::TurnCount::DEFAULT;
};
}  // namespace pokesim
