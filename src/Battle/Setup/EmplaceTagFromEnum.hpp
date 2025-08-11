#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/Nature.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Registry.hpp>

namespace pokesim {
namespace ability::tags {
// Assigns an ability's tag to a handle
void emplaceTagFromEnum(dex::Ability ability, types::handle handle);
}  // namespace ability::tags

namespace item::tags {
// Assigns an item's tag to a handle
void emplaceTagFromEnum(dex::Item item, types::handle handle);
}  // namespace item::tags

namespace nature::tags {
// Assigns a nature's tag to a handle
void emplaceTagFromEnum(dex::Nature nature, types::handle handle);
}  // namespace nature::tags

namespace status::tags {
// Assigns a status' tag to a handle
void emplaceTagFromEnum(dex::Status status, types::handle handle);
}  // namespace status::tags
}  // namespace pokesim