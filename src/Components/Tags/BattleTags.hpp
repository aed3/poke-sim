#pragma once

namespace pokesim::tags {
struct Battle {};
struct Side {};

// Battle Turn State Tag: When a battle is in the middle of a turn
struct BattleMidTurn {};
// Battle Turn State Tag: When a battle has ended
struct BattleEnded {};
}  // namespace pokesim::tags
