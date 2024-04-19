#include "MoveHitSteps.hpp"

#include "RandomChance.hpp"
#include "RunEvent.hpp"
#include "Simulation.hpp"
#include <Components/MultiHit.hpp>
#include <CalcDamage/CalcDamage.hpp>

namespace pokesim {
void setMoveHitCount(Simulation& simulation) {
  sampleRandomChance(simulation);
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
  }
}

void runMoveHitSteps(Simulation& simulation) {
  accuracyCheckStep(simulation);
}
}  // namespace pokesim