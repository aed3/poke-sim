#include "AnalyzeEffect.hpp"

#include <Battle/Clone/Clone.hpp>
#include <CalcDamage/CalcDamage.hpp>
#include <Components/AnalyzeEffect/RemovedEffect.hpp>
#include <Simulation/Simulation.hpp>

namespace pokesim::analyze_effect {
void run(Simulation& simulation) {
  assignAttackerDefender(simulation);

  if (simulation.analyzeEffectOptions.reconsiderActiveEffects) {
    removeConsideredEffect(simulation);
  }

  createAppliedEffectBattles(simulation);

  calc_damage::run(simulation);

  if (simulation.analyzeEffectOptions.reconsiderActiveEffects) {
    reapplyConsideredEffect(simulation);
  }

  postRunCleanup(simulation);
}

void assignAttackerDefender(Simulation& /*simulation*/) {}

void createAppliedEffectBattles(Simulation& /*simulation*/) {}

void removeConsideredEffect(Simulation& /*simulation*/) {}

void reapplyConsideredEffect(Simulation& /*simulation*/) {}

void postRunCleanup(Simulation& /*simulation*/) {
  // Remove duplicate states and attacker/defender components
}
}  // namespace pokesim::analyze_effect