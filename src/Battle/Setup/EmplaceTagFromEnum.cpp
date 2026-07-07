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

namespace pokesim::internal {
namespace {
template <typename Tag>
struct EmplaceTag {
  static void run(types::handle handle) { handle.emplace<Tag>(); }
};
}  // namespace

void ability::tags::emplaceTagFromEnum(pokesim::dex::Ability ability, types::handle handle) {
  pokesim::ability::tags::enumToTag<EmplaceTag>(ability, handle);
}

void item::tags::emplaceTagFromEnum(pokesim::dex::Item item, types::handle handle) {
  pokesim::item::tags::enumToTag<EmplaceTag>(item, handle);
}

void nature::tags::emplaceTagFromEnum(pokesim::dex::Nature nature, types::handle handle) {
  pokesim::nature::tags::enumToTag<EmplaceTag>(nature, handle);
}

void status::tags::emplaceTagFromEnum(pokesim::dex::Status status, types::handle handle) {
  pokesim::status::tags::enumToTag<EmplaceTag>(status, handle);
}

void type::tags::emplaceTagFromEnum(pokesim::dex::Type type, types::handle handle) {
  pokesim::type::tags::enumToTag<EmplaceTag>(type, handle);
}

void move::tags::emplaceTagFromEnum(pokesim::dex::Move move, types::handle handle) {
  pokesim::move::tags::enumToTag<EmplaceTag>(move, handle);
}
}  // namespace pokesim::internal
