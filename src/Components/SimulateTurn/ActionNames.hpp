#pragma once
#include <Components/Names/ItemNames.hpp>
#include <Components/Names/MoveNames.hpp>

namespace pokesim::action {
// Component to make an action one where a Pokemon uses a move. Contains the name of the move being used.
struct Move : MoveName {};
// Component to make an action one where a player uses an item. Contains the name of the item being used.
struct Item : ItemName {};
}  // namespace pokesim::action