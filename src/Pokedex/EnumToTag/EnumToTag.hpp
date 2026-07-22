#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Nature.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Registry.hpp>

namespace pokesim {
namespace ability::tags {
// Assigns an ability's tag to a handle
void emplaceTagFromEnum(pokesim::dex::Ability ability, types::handle handle);
void emplaceTagFromEnum(pokesim::dex::Ability ability, types::registry& registry, types::entity entity);
// Checks if the handle has the ability's tag
bool hasTag(pokesim::dex::Ability ability, types::handle handle);
bool hasTag(pokesim::dex::Ability ability, const types::registry& registry, types::entity entity);
}  // namespace ability::tags

namespace item::tags {
// Assigns an item's tag to a handle
void emplaceTagFromEnum(pokesim::dex::Item item, types::handle handle);
void emplaceTagFromEnum(pokesim::dex::Item item, types::registry& registry, types::entity entity);
// Checks if the handle has the item's tag
bool hasTag(pokesim::dex::Item item, types::handle handle);
bool hasTag(pokesim::dex::Item item, const types::registry& registry, types::entity entity);
}  // namespace item::tags

namespace nature::tags {
// Assigns a nature's tag to a handle
void emplaceTagFromEnum(pokesim::dex::Nature nature, types::handle handle);
void emplaceTagFromEnum(pokesim::dex::Nature nature, types::registry& registry, types::entity entity);
// Checks if the handle has the nature's tag
bool hasTag(pokesim::dex::Nature nature, types::handle handle);
bool hasTag(pokesim::dex::Nature nature, const types::registry& registry, types::entity entity);
}  // namespace nature::tags

namespace status::tags {
// Assigns a status' tag to a handle
void emplaceTagFromEnum(pokesim::dex::Status status, types::handle handle);
void emplaceTagFromEnum(pokesim::dex::Status status, types::registry& registry, types::entity entity);
// Checks if the handle has the status' tag
bool hasTag(pokesim::dex::Status status, types::handle handle);
bool hasTag(pokesim::dex::Status status, const types::registry& registry, types::entity entity);
}  // namespace status::tags

namespace type::tags {
// Assigns a type's tag to a handle
void emplaceTagFromEnum(pokesim::dex::Type type, types::handle handle);
void emplaceTagFromEnum(pokesim::dex::Type type, types::registry& registry, types::entity entity);
// Checks if the handle has the type's tag
bool hasTag(pokesim::dex::Type type, types::handle handle);
bool hasTag(pokesim::dex::Type type, const types::registry& registry, types::entity entity);
}  // namespace type::tags

namespace move::tags {
// Assigns a move's tag to a handle
void emplaceTagFromEnum(pokesim::dex::Move move, types::handle handle);
void emplaceTagFromEnum(pokesim::dex::Move move, types::registry& registry, types::entity entity);
// Checks if the handle has the move's tag
bool hasTag(pokesim::dex::Move move, types::handle handle);
bool hasTag(pokesim::dex::Move move, const types::registry& registry, types::entity entity);
}  // namespace move::tags
}  // namespace pokesim
