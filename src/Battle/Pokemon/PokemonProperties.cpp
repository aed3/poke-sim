#include "PokemonProperties.hpp"

#include <Components/Tags/MovePropertyTags.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::internal {
bool doesMoveMakeContact(types::registry& registry, types::entity move, types::entity) {
  /*
  if (registry.any_of<dex::ProtectivePads>(source)) {
    return false;
  }
  */
  return registry.all_of<move::tags::Contact>(move);
}
}  // namespace pokesim::internal
