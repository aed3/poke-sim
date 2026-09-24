#include "PokemonProperties.hpp"

#include <Components/EntityHolders/Current.hpp>
#include <Components/Names/TypeNames.hpp>
#include <Components/SpeciesTypes.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Pokedex/Pokedex.hpp>
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

bool isGrounded(types::registry&, types::entity) {
  return true;
}

bool isTargetImmune(types::registry& registry, CurrentActionTarget target, TypeName typeName, const Pokedex& pokedex) {
  bool targetImmune = isSpeciesTypeImmune(registry.get<SpeciesTypes>(target.val), typeName.val, pokedex.typeChart());
  if (!targetImmune && typeName.val == dex::Type::GROUND) {
    return !isGrounded(registry, target.val);
  }
  return targetImmune;
}
}  // namespace pokesim::internal
