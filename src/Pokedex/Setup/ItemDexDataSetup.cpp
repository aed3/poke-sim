#include "ItemDexDataSetup.hpp"

#include <Components/Names/ItemNames.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim::dex::internal {
void ItemDexDataSetup::setName(Item item) {
  handle.emplace<ItemName>(item);
}
}  // namespace pokesim::dex::internal