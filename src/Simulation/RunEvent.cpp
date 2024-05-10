#include "RunEvent.hpp"

#include <Components/Stats.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Pokedex/Abilities/headers.hpp>
#include <Types/Enums/GameMechanics.hpp>

#include "Simulation.hpp"

namespace pokesim {
void runAccuracyEvent(Simulation& /*simulation*/) {}

void runModifyAccuracyEvent(Simulation& /*simulation*/) {}

void runModifyCritRatioEvent(Simulation& /*simulation*/) {}

void runBasePowerEvent(Simulation& /*simulation*/) {}

void runDamagingHitEvent(Simulation& simulation) {
  dex::latest::Static::onDamagingHit(simulation);
}

void runModifySpe(Simulation& simulation) {
  // simulation.view<function, ...Tags>();

  simulation.viewForSelectedPokemon<dex::latest::Static::onModifySpe, status::tags::Paralysis>(
    /*entt::exclude_t<ability::tags::QuickFeet>{}*/);
}
}  // namespace pokesim