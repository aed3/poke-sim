#pragma once

namespace pokesim {

// Current Action Tags

struct ActiveMove {};
struct ActiveMoveTarget {};
struct ActiveMoveSource {};

// Turn State

struct MidTurn {};
struct Started {};
struct Ended {};
}  // namespace pokesim
