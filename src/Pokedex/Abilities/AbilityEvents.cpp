#include <Components/Stats.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/GameMechanics.hpp>

#include "headers.hpp"

namespace pokesim::dex {
void internal::StaticEvents::onDamagingHit(Simulation& /*simulation*/) {}
void internal::StaticEvents::onModifySpe(stat::EffectiveSpe& effectiveSpe) {
  effectiveSpe.val /= 2U;
}
}  // namespace pokesim::dex