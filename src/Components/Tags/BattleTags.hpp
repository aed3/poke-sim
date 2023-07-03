#pragma once

namespace pokesim::tags {

// Current Action Tag: The move that is being processed by the simulator
struct ActiveMove {};
// Current Action Tag: The target of the active move
struct ActiveMoveTarget {};
// Current Action Tag: The user of the active move
struct ActiveMoveUser {};

// Battle Turn State Tag: When a battle is in the middle of a turn
struct BattleMidTurn {};
// Battle Turn State Tag: When a battle has ended
struct BattleEnded {};
}  // namespace pokesim::tags
