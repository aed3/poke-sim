#include <Components/Stats.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/GameMechanics.hpp>

#include "headers.hpp"

namespace pokesim::dex::internal {
void PlusEvents::onModifySpA(types::handle, EventModifier&) {}

void StaticEvents::onDamagingHit(Simulation&) {}
void StaticEvents::onModifySpe(stat::EffectiveSpe& effectiveSpe) {
  effectiveSpe.val /= 2U;
}
}  // namespace pokesim::dex::internal