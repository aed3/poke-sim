#include "ManageSideState.hpp"

#include <Components/EntityHolders/FoeSide.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Types/Entity.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
types::teamPositionIndex sidePokemonLeft(const types::registry& registry, types::entity sideEntity) {
  const Team& team = registry.get<Team>(sideEntity);
  types::teamPositionIndex pokemonLeft = 0U;
  for (types::entity pokemon : team.val) {
    if (!registry.all_of<tags::Fainted>(pokemon)) {
      pokemonLeft++;
    }
  }
  return pokemonLeft;
}

types::teamPositionIndex foeSidePokemonLeft(const types::registry& registry, types::entity sideEntity) {
  return sidePokemonLeft(registry, registry.get<FoeSide>(sideEntity).val);
}
}  // namespace pokesim
