#pragma once
#include <Types/Enums/Item.hpp>
#include <entt/entity/fwd.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::tags::item {
struct AssaultVest {};
struct BrightPowder {};
struct ChoiceScarf {};
struct ChoiceSpecs {};
struct FocusSash {};
struct LifeOrb {};

// Assigns an item's tag to a handle
/*_inline_*/ void enumToTag(dex::Item item, entt::handle handle);
}  // namespace pokesim::tags::item
