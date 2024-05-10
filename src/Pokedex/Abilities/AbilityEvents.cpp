#include <Components/Stats.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Enums/GameMechanics.hpp>

#include "headers.hpp"

namespace pokesim::dex {
void internal::StaticEvents::onDamagingHit(Simulation& /*simulation*/) {}
void internal::StaticEvents::onModifySpe(stat::EffectiveSpeed& effectiveSpeed) {
  effectiveSpeed.effectiveSpeed /= 2;
}
}  // namespace pokesim::dex