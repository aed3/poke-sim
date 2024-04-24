#include "MoveHitSteps.hpp"

#include <CalcDamage/CalcDamage.hpp>
#include <Components/MultiHit.hpp>

#include "RandomChance.hpp"
#include "RunEvent.hpp"
#include "Simulation.hpp"

namespace pokesim {
void setMoveHitCount(Simulation& simulation) {
  sampleRandomChance(simulation);
}

void applyDamage(Simulation& /*simulation*/) {}

void trySetStatusFromEffect(Simulation& /*simulation*/) {}

void runSecondaryMoveEffects(Simulation& simulation) {
  // Set secondary effect of active move

  trySetStatusFromEffect(simulation);
}

void accuracyRandomChance(Simulation& simulation) {
  // Set accuracies as random chance variable

  randomChance(simulation);
}

void accuracyCheckStep(Simulation& simulation) {
  runModifyAccuracyEvent(simulation);
  runAccuracyEvent(simulation);

  accuracyRandomChance(simulation);
}

void moveHitStep(Simulation& simulation) {
  setMoveHitCount(simulation);

  while (!simulation.registry.view<HitCount>().empty()) {
    calc_damage::run(simulation);

    // for simulate turn only
    applyDamage(simulation);
    runSecondaryMoveEffects(simulation);
    runDamagingHitEvent(simulation);
  }
}

void runMoveHitSteps(Simulation& simulation) {
  accuracyCheckStep(simulation);
}
}  // namespace pokesim