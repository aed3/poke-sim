#include "AbilityDexDataSetup.hpp"

#include <Components/Names/AbilityNames.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim::internal::dex {
void AbilityDexDataSetup::setName(pokesim::dex::Ability ability) {
  handle.emplace<AbilityName>(ability);
}
}  // namespace pokesim::internal::dex
