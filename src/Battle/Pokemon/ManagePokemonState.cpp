#include "ManagePokemonState.hpp"

#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/LastUsedMove.hpp>
#include <Components/PP.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Registry.hpp>
#include <Types/Entity.hpp>
#include <Utilities/SelectForView.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
void deductPp(Pp& pp) {
  if (pp.val) {
    pp.val -= 1;
  }
}

void setLastMoveUsed(types::registry& registry, const CurrentActionSource& source, const CurrentActionMoveSlot& move) {
  registry.emplace<LastUsedMove>(source.val, move.val);
}

void resetEffectiveSpeed(types::handle handle, stat::Spe spe) {
  handle.emplace_or_replace<stat::EffectiveSpeed>(spe.val);
}

void updateSpeed(Simulation& simulation) {
  internal::SelectForPokemonView<tags::SpeedUpdateRequired> selectedSpeedUpdateRequired{simulation};
  if (selectedSpeedUpdateRequired.hasNoneSelected()) return;

  simulation.viewForSelectedPokemon<resetEffectiveSpeed>();

  // apply boosts
  runModifySpe(simulation);
  // trick room

  selectedSpeedUpdateRequired.deselect();
  simulation.registry.clear<tags::SpeedUpdateRequired>();
}
}  // namespace pokesim