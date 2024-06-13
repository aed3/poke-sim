#include "MoveHitSteps.hpp"

#include <CalcDamage/CalcDamage.hpp>
#include <Components/Accuracy.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/MultiHit.hpp>
#include <Components/Tags/BattleTags.hpp>

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

void assignAccuracyToRandomEvent(
  types::registry& registry, const Simulation& simulation, const Battle& battle, const Accuracy& accuracy) {
  setRandomBinaryChoice(simulation, {registry, battle.battle}, accuracy.accuracy);
}

void accuracyCheck(Simulation& simulation) {
  runModifyAccuracyEvent(simulation);
  runAccuracyEvent(simulation);

  simulation.view<assignAccuracyToRandomEvent, tags::CurrentActionMove>();
  randomChance<2U>(simulation);
}

void moveHitLoop(Simulation& simulation) {
  setMoveHitCount(simulation);

  while (!simulation.registry.view<HitCount>().empty()) {
    calc_damage::run(simulation);

    // for simulate turn only
    applyDamage(simulation);
    runSecondaryMoveEffects(simulation);
    runDamagingHitEvent(simulation);

    // Update stats if needed
  }
}

void runMoveHitSteps(Simulation& simulation) {
  // invulnerabilityCheck
  // hitCheck
  // immunityCheck
  accuracyCheck(simulation);
  // breakProtectCheck
  // stealBoostCheck
  moveHitLoop(simulation);
}
}  // namespace pokesim