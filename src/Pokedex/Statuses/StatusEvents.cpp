#include <Components/Stats.hpp>
#include <Types/Enums/GameMechanics.hpp>

#include "headers.hpp"

namespace pokesim::dex::internal {
void ParalysisEvents::onModifySpe(stat::EffectiveSpe& effectiveSpe) {
  effectiveSpe.val /= latest::Paralysis::onModifySpaModifier;
}
}  // namespace pokesim::dex::internal