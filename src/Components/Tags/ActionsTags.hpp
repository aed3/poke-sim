#pragma once

namespace pokesim::action::tags {
// Action Tag for Moves: When a Pokemon uses a move
struct Move {};

// Action Tag for Switching: When a Pokemon is about to be switched out
// Example: Pursuit activating
struct PreSwitchOut {};
// Action Tag for Switching: When a Pokemon is being switched out
struct SwitchOut {};
/**
 * @brief Action Tag for Switching: When a Pokemon is being switched in because a player chose to directly switch the
 * Pokemon in as their turn action
 */
struct PreTurnSwitchIn {};
// Action Tag for Switching: When a Pokemon is being switched in because a team member switches out mid-turn
// Examples: U-Turn, Baton Pash, Emergency Exit
struct MidTurnSwitchIn {};
// Action Tag for Switching: When a Pokemon is being switched in to replace a fainted team member
struct PostFoeFaintSwitchIn {};

// Action Tag: When an item is being used because a player chose to use the item as their turn action
struct Item {};
// Action Tag: When team member order has been picked as part of team preview
struct Team {};
// Action Tag: When Revival Blessing is being used on a fainted party member
struct RevivalBlessing {};
// Action Tag: When a mega evolution is activating
struct MegaEvolve {};
// Action Tag: When a primal reversion is activating
struct PrimalRevert {};
// Action Tag: When a Pokemon dynamaxing
struct Dynamax {};
// Action Tag: When a Pokemon is terastallizing
struct Terastallize {};
}  // namespace pokesim::action::tags
