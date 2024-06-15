#pragma once

namespace pokesim::tags {

// Current Action Tag: The move that is being processed by the simulator
struct CurrentActionMove {};
// Current Action Tag: The move slot the current action's move was chosen and will deduct PP from
struct CurrentActionMoveSlot {};
// Current Action Tag: The target of the active move
struct CurrentActionMoveTarget {};
// Current Action Tag: The user of the active move
struct CurrentActionMoveSource {};

// Battle Turn State Tag: When a battle is in the middle of a turn
struct BattleMidTurn {};
// Battle Turn State Tag: When a battle has ended
struct BattleEnded {};
}  // namespace pokesim::tags
