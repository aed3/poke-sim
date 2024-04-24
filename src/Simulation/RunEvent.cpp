#include "RunEvent.hpp"

#include <Pokedex/Abilities/headers.hpp>
#include <Types/Enums/GameMechanics.hpp>

#include "Simulation.hpp"

namespace pokesim {
void runAccuracyEvent(Simulation& /*simulation*/) {}

void runModifyAccuracyEvent(Simulation& /*simulation*/) {}

void runModifyCritRatioEvent(Simulation& /*simulation*/) {}

void runBasePowerEvent(Simulation& /*simulation*/) {}

void runDamagingHitEvent(Simulation& simulation) {
  dex::Static<GameMechanics::NONE>::onDamagingHit(simulation);
}
}  // namespace pokesim