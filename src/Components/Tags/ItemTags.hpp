#pragma once
#include <Types/Entity.hpp>
#include <Types/Enums/Item.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::tags::item {
struct AssaultVest {};
struct BrightPowder {};
struct ChoiceScarf {};
struct ChoiceSpecs {};
struct FocusSash {};
struct LifeOrb {};

// Assigns an item's tag to a handle
/*_inline_*/ void enumToTag(dex::Item item, types::handle handle);
}  // namespace pokesim::tags::item
