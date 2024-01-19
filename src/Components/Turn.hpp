#pragma once

#include <Types/State.hpp>

namespace pokesim {
// The current turn of a battle
struct Turn {
  types::battleTurn turn = 0;
};
}  // namespace pokesim