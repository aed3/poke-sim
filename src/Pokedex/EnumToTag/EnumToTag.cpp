#include <Pokedex/EnumToTag/headers.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Nature.hpp>
#include <Types/Enums/SideCondition.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::dex {
namespace {
template <typename Tag>
struct EmplaceTag {
  static void run(types::registry& registry, types::entity entity) { registry.emplace<Tag>(entity); }
};

template <typename Tag>
struct RemoveTag {
  static void run(types::registry& registry, types::entity entity) { registry.remove<Tag>(entity); }
};

template <typename Tag>
struct HasTag {
  static bool run(const types::registry& registry, types::entity entity) { return registry.all_of<Tag>(entity); }
};

template <typename EnumType>
void callEmplace(EnumType enumValue, types::registry& registry, types::entity entity) {
  enumToTag<EmplaceTag>(enumValue, registry, entity);
}

template <typename EnumType>
void callEmplace(EnumType enumValue, types::handle handle) {
  callEmplace(enumValue, *handle.registry(), handle.entity());
}

template <typename EnumType>
void callRemove(EnumType enumValue, types::registry& registry, types::entity entity) {
  enumToTag<RemoveTag>(enumValue, registry, entity);
}

template <typename EnumType>
void callRemove(EnumType enumValue, types::handle handle) {
  callRemove(enumValue, *handle.registry(), handle.entity());
}

template <typename EnumType>
bool callHasTag(EnumType enumValue, const types::registry& registry, types::entity entity) {
  return enumToTag<HasTag>(enumValue, registry, entity);
}

template <typename EnumType>
bool callHasTag(EnumType enumValue, types::handle handle) {
  return callHasTag(enumValue, *handle.registry(), handle.entity());
}
}  // namespace

// clang-format off
void emplaceTagFromEnum(Ability ability, types::handle handle) { callEmplace(ability, handle); }
void emplaceTagFromEnum(AbilityProperty property, types::handle handle) { callEmplace(property, handle); }
void emplaceTagFromEnum(Item item, types::handle handle) { callEmplace(item, handle); }
void emplaceTagFromEnum(ItemProperty property, types::handle handle) { callEmplace(property, handle); }
void emplaceTagFromEnum(Move move, types::handle handle) { callEmplace(move, handle); }
void emplaceTagFromEnum(MoveProperty property, types::handle handle) { callEmplace(property, handle); }
void emplaceTagFromEnum(Nature nature, types::handle handle) { callEmplace(nature, handle); }
void emplaceTagFromEnum(SideCondition sideCondition, types::handle handle) { callEmplace(sideCondition, handle); }
void emplaceTagFromEnum(Status status, types::handle handle) { callEmplace(status, handle); }
void emplaceTagFromEnum(Type type, types::handle handle) { callEmplace(type, handle); }

void emplaceTagFromEnum(Ability ability, types::registry& registry, types::entity entity) { callEmplace(ability, registry, entity); }
void emplaceTagFromEnum(AbilityProperty property, types::registry& registry, types::entity entity) { callEmplace(property, registry, entity); }
void emplaceTagFromEnum(Item item, types::registry& registry, types::entity entity) { callEmplace(item, registry, entity); }
void emplaceTagFromEnum(ItemProperty property, types::registry& registry, types::entity entity) { callEmplace(property, registry, entity); }
void emplaceTagFromEnum(Move move, types::registry& registry, types::entity entity) { callEmplace(move, registry, entity); }
void emplaceTagFromEnum(MoveProperty property, types::registry& registry, types::entity entity) { callEmplace(property, registry, entity); }
void emplaceTagFromEnum(Nature nature, types::registry& registry, types::entity entity) { callEmplace(nature, registry, entity); }
void emplaceTagFromEnum(SideCondition sideCondition, types::registry& registry, types::entity entity) { callEmplace(sideCondition, registry, entity); }
void emplaceTagFromEnum(Status status, types::registry& registry, types::entity entity) { callEmplace(status, registry, entity); }
void emplaceTagFromEnum(Type type, types::registry& registry, types::entity entity) { callEmplace(type, registry, entity); }

void removeTagFromEnum(Ability ability, types::handle handle) { callRemove(ability, handle); }
void removeTagFromEnum(AbilityProperty property, types::handle handle) { callRemove(property, handle); }
void removeTagFromEnum(Item item, types::handle handle) { callRemove(item, handle); }
void removeTagFromEnum(ItemProperty property, types::handle handle) { callRemove(property, handle); }
void removeTagFromEnum(Move move, types::handle handle) { callRemove(move, handle); }
void removeTagFromEnum(MoveProperty property, types::handle handle) { callRemove(property, handle); }
void removeTagFromEnum(Nature nature, types::handle handle) { callRemove(nature, handle); }
void removeTagFromEnum(SideCondition sideCondition, types::handle handle) { callRemove(sideCondition, handle); }
void removeTagFromEnum(Status status, types::handle handle) { callRemove(status, handle); }
void removeTagFromEnum(Type type, types::handle handle) { callRemove(type, handle); }

void removeTagFromEnum(Ability ability, types::registry& registry, types::entity entity) { callRemove(ability, registry, entity); }
void removeTagFromEnum(AbilityProperty property, types::registry& registry, types::entity entity) { callRemove(property, registry, entity); }
void removeTagFromEnum(Item item, types::registry& registry, types::entity entity) { callRemove(item, registry, entity); }
void removeTagFromEnum(ItemProperty property, types::registry& registry, types::entity entity) { callRemove(property, registry, entity); }
void removeTagFromEnum(Move move, types::registry& registry, types::entity entity) { callRemove(move, registry, entity); }
void removeTagFromEnum(MoveProperty property, types::registry& registry, types::entity entity) { callRemove(property, registry, entity); }
void removeTagFromEnum(Nature nature, types::registry& registry, types::entity entity) { callRemove(nature, registry, entity); }
void removeTagFromEnum(SideCondition sideCondition, types::registry& registry, types::entity entity) { callRemove(sideCondition, registry, entity); }
void removeTagFromEnum(Status status, types::registry& registry, types::entity entity) { callRemove(status, registry, entity); }
void removeTagFromEnum(Type type, types::registry& registry, types::entity entity) { callRemove(type, registry, entity); }

bool hasTag(Ability ability, types::handle handle) { return callHasTag(ability, handle); }
bool hasTag(Item item, types::handle handle) { return callHasTag(item, handle); }
bool hasTag(Move move, types::handle handle) { return callHasTag(move, handle); }
bool hasTag(Nature nature, types::handle handle) { return callHasTag(nature, handle); }
bool hasTag(SideCondition sideCondition, types::handle handle) { return callHasTag(sideCondition, handle); }
bool hasTag(Status status, types::handle handle) { return callHasTag(status, handle); }
bool hasTag(Type type, types::handle handle) { return callHasTag(type, handle); }

bool hasTag(Ability ability, const types::registry& registry, types::entity entity) { return callHasTag(ability, registry, entity); }
bool hasTag(Item item, const types::registry& registry, types::entity entity) { return callHasTag(item, registry, entity); }
bool hasTag(Move move, const types::registry& registry, types::entity entity) { return callHasTag(move, registry, entity); }
bool hasTag(Nature nature, const types::registry& registry, types::entity entity) { return callHasTag(nature, registry, entity); }
bool hasTag(SideCondition sideCondition, const types::registry& registry, types::entity entity) { return callHasTag(sideCondition, registry, entity); }
bool hasTag(Status status, const types::registry& registry, types::entity entity) { return callHasTag(status, registry, entity); }
bool hasTag(Type type, const types::registry& registry, types::entity entity) { return callHasTag(type, registry, entity); }
// clang-format on
}  // namespace pokesim::dex
