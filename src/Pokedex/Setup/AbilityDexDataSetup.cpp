#include "AbilityDexDataSetup.hpp"

#include <Components/Names/AbilityNames.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim::dex::internal {
void AbilityDexDataSetup::setName(Ability ability) {
  handle.emplace<AbilityName>(ability);
}
}  // namespace pokesim::dex::internal