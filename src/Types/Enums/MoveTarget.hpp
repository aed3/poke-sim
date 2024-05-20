#pragma once

#include <cstdint>

namespace pokesim {
enum class MoveTarget : std::uint8_t {
  NONE = 0,
  ANY_SINGLE_TARGET,  // Targets one Pokemon other than the move's user
  ANY_SINGLE_FOE,     // Targets one foe
  ANY_SINGLE_ALLY,    // Targets one ally
  ALLY_OR_SELF,       // Targets either one ally or the move's user
  SELF,               // Targets the move's user

  ALL_FOES,         // Targets every active foe
  ALLIES_AND_FOES,  // Targets every active foe and the move user's allies
  ALLIES_AND_SELF,  // Targets the move user's allies and the user

  FOE_SIDE,   // Targets the opponent's side
  ALLY_SIDE,  // Targets the move user's side
  FIELD,      // Targets the field or all Pokemon at once

  ALLY_TEAM,    // Targets all non-fainted Pokemon on the user's team
  RETALIATION,  // Targets the last Pokemon to damage the user during a turn
  RANDOM_FOE,   // Targets a random foe
};
}