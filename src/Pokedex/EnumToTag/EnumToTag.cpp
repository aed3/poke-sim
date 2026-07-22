#include "EnumToTag.hpp"

#include <Pokedex/EnumToTag/headers.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Nature.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
namespace {
template <typename Tag>
struct EmplaceTag {
  static void run(types::registry& registry, types::entity entity) { registry.emplace<Tag>(entity); }
};

template <typename Tag>
struct HasTag {
  static bool run(const types::registry& registry, types::entity entity) { return registry.all_of<Tag>(entity); }
};
}  // namespace

namespace ability::tags {
void emplaceTagFromEnum(pokesim::dex::Ability ability, types::handle handle) {
  emplaceTagFromEnum(ability, *handle.registry(), handle.entity());
}
void emplaceTagFromEnum(dex::Ability ability, types::registry& registry, types::entity entity) {
  enumToTag<EmplaceTag>(ability, registry, entity);
}

bool hasTag(dex::Ability ability, types::handle handle) {
  return hasTag(ability, *handle.registry(), handle.entity());
}
bool hasTag(dex::Ability ability, const types::registry& registry, types::entity entity) {
  return enumToTag<HasTag>(ability, registry, entity);
}
}  // namespace ability::tags

namespace item::tags {
void emplaceTagFromEnum(dex::Item item, types::handle handle) {
  emplaceTagFromEnum(item, *handle.registry(), handle.entity());
}
void emplaceTagFromEnum(dex::Item item, types::registry& registry, types::entity entity) {
  enumToTag<EmplaceTag>(item, registry, entity);
}

bool hasTag(dex::Item item, types::handle handle) {
  return hasTag(item, *handle.registry(), handle.entity());
}
bool hasTag(dex::Item item, const types::registry& registry, types::entity entity) {
  return enumToTag<HasTag>(item, registry, entity);
}
}  // namespace item::tags

namespace nature::tags {
void emplaceTagFromEnum(dex::Nature nature, types::handle handle) {
  emplaceTagFromEnum(nature, *handle.registry(), handle.entity());
}
void emplaceTagFromEnum(dex::Nature nature, types::registry& registry, types::entity entity) {
  enumToTag<EmplaceTag>(nature, registry, entity);
}

bool hasTag(dex::Nature nature, types::handle handle) {
  return hasTag(nature, *handle.registry(), handle.entity());
}
bool hasTag(dex::Nature nature, const types::registry& registry, types::entity entity) {
  return enumToTag<HasTag>(nature, registry, entity);
}
}  // namespace nature::tags

namespace status::tags {
void emplaceTagFromEnum(dex::Status status, types::handle handle) {
  emplaceTagFromEnum(status, *handle.registry(), handle.entity());
}
void emplaceTagFromEnum(dex::Status status, types::registry& registry, types::entity entity) {
  enumToTag<EmplaceTag>(status, registry, entity);
}

bool hasTag(dex::Status status, types::handle handle) {
  return hasTag(status, *handle.registry(), handle.entity());
}
bool hasTag(dex::Status status, const types::registry& registry, types::entity entity) {
  return enumToTag<HasTag>(status, registry, entity);
}
}  // namespace status::tags

namespace type::tags {
void emplaceTagFromEnum(dex::Type type, types::handle handle) {
  emplaceTagFromEnum(type, *handle.registry(), handle.entity());
}
void emplaceTagFromEnum(dex::Type type, types::registry& registry, types::entity entity) {
  enumToTag<EmplaceTag>(type, registry, entity);
}

bool hasTag(dex::Type type, types::handle handle) {
  return hasTag(type, *handle.registry(), handle.entity());
}
bool hasTag(dex::Type type, const types::registry& registry, types::entity entity) {
  return enumToTag<HasTag>(type, registry, entity);
}
}  // namespace type::tags

namespace move::tags {
void emplaceTagFromEnum(dex::Move move, types::handle handle) {
  emplaceTagFromEnum(move, *handle.registry(), handle.entity());
}
void emplaceTagFromEnum(dex::Move move, types::registry& registry, types::entity entity) {
  enumToTag<EmplaceTag>(move, registry, entity);
}

bool hasTag(dex::Move move, types::handle handle) {
  return hasTag(move, *handle.registry(), handle.entity());
}
bool hasTag(dex::Move move, const types::registry& registry, types::entity entity) {
  return enumToTag<HasTag>(move, registry, entity);
}
}  // namespace move::tags
}  // namespace pokesim
