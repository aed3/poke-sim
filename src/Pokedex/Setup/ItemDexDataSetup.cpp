#include "ItemDexDataSetup.hpp"

#include <Components/Names/ItemNames.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim::internal::dex {
void ItemDexDataSetup::setName(pokesim::dex::Item item) {
  handle.emplace<ItemName>(item);
}
}  // namespace pokesim::internal::dex
