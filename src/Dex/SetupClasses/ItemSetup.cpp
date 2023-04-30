#include "ItemSetup.hpp"

namespace pokesim::dex::internal {
void ItemSetup::setName(Item item) {
  handle.emplace<ItemName>(item);
}
}  // namespace pokesim::dex::internal