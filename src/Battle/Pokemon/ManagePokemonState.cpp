#include "ManagePokemonState.hpp"

#include <Components/PP.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
void deductPp(Pp& /*pp*/) {}
void setLastMoveUsed(types::handle /*handle*/) {}
void resetEffectiveSpeed(types::handle handle, stat::Spe spe) {
  handle.emplace_or_replace<stat::EffectiveSpeed>(spe.stat);
}

void updateSpeed(Simulation& simulation) {
  simulation.view<resetEffectiveSpeed, tags::SpeedUpdateRequired>();
  // apply boosts
  runModifySpe(simulation);
  // trick room

  simulation.registry.clear<tags::SpeedUpdateRequired>();
}
}  // namespace pokesim