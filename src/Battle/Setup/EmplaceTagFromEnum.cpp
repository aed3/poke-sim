#include "EmplaceTagFromEnum.hpp"

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
  static void run(types::handle handle) { handle.emplace<Tag>(); }
};
}  // namespace

void ability::tags::emplaceTagFromEnum(dex::Ability ability, types::handle handle) {
  enumToTag<EmplaceTag>(ability, handle);
}

void item::tags::emplaceTagFromEnum(dex::Item item, types::handle handle) {
  enumToTag<EmplaceTag>(item, handle);
}

void nature::tags::emplaceTagFromEnum(dex::Nature nature, types::handle handle) {
  enumToTag<EmplaceTag>(nature, handle);
}

void status::tags::emplaceTagFromEnum(dex::Status status, types::handle handle) {
  enumToTag<EmplaceTag>(status, handle);
}

void type::tags::emplaceTagFromEnum(dex::Type type, types::handle handle) {
  enumToTag<EmplaceTag>(type, handle);
}

void move::tags::emplaceTagFromEnum(dex::Move move, types::handle handle) {
  enumToTag<EmplaceTag>(move, handle);
}
}  // namespace pokesim
