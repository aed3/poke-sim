#include "RunEvent.hpp"

#include <Components/Stats.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Pokedex/Abilities/headers.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Utilities/Tags.hpp>

#include "Simulation.hpp"

namespace pokesim {
void runAccuracyEvent(Simulation& /*simulation*/) {}

void runModifyAccuracyEvent(Simulation& /*simulation*/) {}

void runModifyCritBoostEvent(Simulation& /*simulation*/) {}

void runBasePowerEvent(Simulation& /*simulation*/) {}

void runDamagingHitEvent(Simulation& simulation) {
  dex::latest::Static::onDamagingHit(simulation);
}

void runModifySpe(Simulation& simulation) {
  simulation.viewForSelectedPokemon<
    dex::latest::Static::onModifySpe,
    Tags<status::tags::Paralysis> /*, entt::exclude_t<ability::tags::QuickFeet>*/>();
}
}  // namespace pokesim