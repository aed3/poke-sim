#include "MoveHitSteps.hpp"

#include <CalcDamage/CalcDamage.hpp>
#include <Components/Accuracy.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/MultiHit.hpp>
#include <Components/SimulateTurn/MoveHitStepTags.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/RandomChanceTags.hpp>

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

void internal::assignAccuracyToRandomEvent(
  types::registry& registry, const Simulation& simulation, const Battle& battle, const Accuracy& accuracy) {
  setRandomBinaryChoice(simulation, {registry, battle.val}, accuracy.val);
}

void internal::assignMoveAccuracyToTargets(types::handle targetHandle, const Battle& battle) {
  types::registry& registry = *targetHandle.registry();
  const Accuracy& accuracy = registry.get<Accuracy>(registry.get<CurrentActionMove>(battle.val).val);
  targetHandle.emplace<Accuracy>(accuracy);
}

void internal::removeAccuracyFromTargets(types::registry& registry, const CurrentActionTargets& targets) {
  registry.remove<Accuracy>(targets.val.begin(), targets.val.end());
}

void accuracyCheck(Simulation& simulation) {
  simulation.view<internal::assignMoveAccuracyToTargets, tags::CurrentActionMoveTarget>();
  runModifyAccuracyEvent(simulation);
  runAccuracyEvent(simulation);

  simulation.view<internal::assignAccuracyToRandomEvent, tags::CurrentActionMoveTarget>();
  randomChance<2U>(simulation);

  auto missed = simulation.registry.view<tags::RandomEventCheckFailed, tags::CurrentActionMoveTarget>();
  simulation.registry.remove<tags::internal::TargetCanBeHit>(missed.begin(), missed.end());

  clearRandomChanceResult(simulation);

  simulation.view<internal::removeAccuracyFromTargets>();
  // clear accuracy from target
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

void internal::assignHitTags(types::registry& registry, const CurrentActionTargets& targets) {
  registry.insert<tags::internal::TargetCanBeHit>(targets.val.begin(), targets.val.end());
}

void internal::removeFailedHitTargets(types::registry& registry, CurrentActionTargets& targets) {
  types::targets<types::entity> newTargets;
  for (types::entity target : targets.val) {
    if (!registry.all_of<tags::internal::TargetCanBeHit>(target)) {
      registry.remove<tags::CurrentActionMoveTarget>(target);
    }
    else {
      newTargets.push_back(target);
    }
  }
  targets.val = newTargets;
}

template <auto Function>
void runMoveHitCheck(Simulation& simulation) {
  simulation.view<internal::assignHitTags>();

  Function(simulation);
  // exit if there are no targets
  // assign a hit tag to each target
  // for inside each function, use the hitResult as what's selected and remove the tag as needed when something fails
  // if tag isn't there in the end, remove the target and its CurrentActionMoveTarget tag
  // clear the tags

  simulation.view<internal::removeFailedHitTargets>();
  simulation.registry.clear<tags::internal::TargetCanBeHit>();
}

void runMoveHitChecks(Simulation& simulation) {
  // invulnerabilityCheck
  // hitCheck
  // immunityCheck
  runMoveHitCheck<accuracyCheck>(simulation);
  // breakProtectCheck
  // stealBoostCheck
  runMoveHitCheck<moveHitLoop>(simulation);
}

template void runMoveHitCheck<accuracyCheck>(Simulation& simulation);
template void runMoveHitCheck<moveHitLoop>(Simulation& simulation);
}  // namespace pokesim