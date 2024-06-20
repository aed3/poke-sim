#include "MoveHitSteps.hpp"

#include <CalcDamage/CalcDamage.hpp>
#include <Components/Accuracy.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/HitCount.hpp>
#include <Components/SimulateTurn/MoveHitStepTags.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/RandomChanceTags.hpp>
#include <Types/Enums/BattleFormat.hpp>
#include <Utilities/Tags.hpp>

#include "RandomChance.hpp"
#include "RunEvent.hpp"
#include "Simulation.hpp"

namespace pokesim {
template <BattleFormat Format>
void internal::assignHitCountToTargets(types::handle targetHandle, const CurrentActionMove& currentMove) {
  types::registry& registry = *targetHandle.registry();
  if (registry.all_of<move::tags::VariableHitCount>(currentMove.val)) {
    // The 35%-35%-15%-15% out of 100 for 2-3-4-5 hits added so each index is the sum of the chance of its hit count and
    // the hit counts less than it so it works with the randomChance function
    static constexpr std::array<types::percentChance, 4U> progressiveMultiHitChances = {35U, 70U, 85U, 100U};

    Battle battle = targetHandle.get<Battle>();
    setRandomChoice<4U, Format, false>({registry, battle.val}, progressiveMultiHitChances);
    return;
  }

  const HitCount* hitCount = registry.try_get<HitCount>(currentMove.val);
  targetHandle.emplace<HitCount>(hitCount ? hitCount->val : (types::moveHits)1U);
}

template <types::moveHits MoveHits>
void internal::assignHitCountFromVariableHitChance(types::registry& registry, const CurrentActionTargets& targets) {
  for (types::entity target : targets.val) {
    if (registry.all_of<tags::internal::TargetCanBeHit>(target)) {
      registry.emplace<HitCount>(target, MoveHits);
    }
  }
}

void setMoveHitCount(Simulation& simulation) {
  if (simulation.battleFormat == BattleFormat::SINGLES_BATTLE_FORMAT) {
    simulation.view<
      internal::assignHitCountToTargets<BattleFormat::SINGLES_BATTLE_FORMAT>,
      Tags<tags::internal::TargetCanBeHit>>();
  }
  else {
    simulation.view<
      internal::assignHitCountToTargets<BattleFormat::DOUBLES_BATTLE_FORMAT>,
      Tags<tags::internal::TargetCanBeHit>>();
  }

  if (!simulation.registry.view<RandomEventChances<4U>>().empty()) {
    randomChance<4U>(simulation);
    simulation.view<internal::assignHitCountFromVariableHitChance<2U>, Tags<tags::RandomEventA>>();
    simulation.view<internal::assignHitCountFromVariableHitChance<3U>, Tags<tags::RandomEventB>>();
    simulation.view<internal::assignHitCountFromVariableHitChance<4U>, Tags<tags::RandomEventC>>();
    simulation.view<internal::assignHitCountFromVariableHitChance<5U>, Tags<tags::RandomEventD>>();
    clearRandomChanceResult(simulation);
  }
}

void applyDamage(Simulation& /*simulation*/) {}

void trySetStatusFromEffect(Simulation& /*simulation*/) {}

void runSecondaryMoveEffects(Simulation& simulation) {
  // Set secondary effect of active move

  trySetStatusFromEffect(simulation);
}

void internal::removeFailedAccuracyCheckTargets(types::registry& registry, const CurrentActionTargets& targets) {
  for (types::entity target : targets.val) {
    if (registry.all_of<tags::internal::TargetCanBeHit>(target)) {
      registry.remove<tags::internal::TargetCanBeHit>(target);
    }
  }
}

void internal::assignMoveAccuracyToTargets(types::handle targetHandle, const CurrentActionMove& currentMove) {
  const Accuracy& accuracy = targetHandle.registry()->get<Accuracy>(currentMove.val);
  targetHandle.emplace<Accuracy>(accuracy);
}

void internal::removeAccuracyFromTargets(types::registry& registry, const CurrentActionTargets& targets) {
  registry.remove<Accuracy>(targets.val.begin(), targets.val.end());
}

void accuracyCheck(Simulation& simulation) {
  simulation.view<internal::assignMoveAccuracyToTargets, Tags<tags::internal::TargetCanBeHit>>();
  runModifyAccuracyEvent(simulation);
  runAccuracyEvent(simulation);

  setRandomBinaryChoice<Accuracy, tags::internal::TargetCanBeHit>(simulation);
  randomBinaryChance(simulation);
  simulation.view<internal::removeFailedAccuracyCheckTargets, Tags<tags::RandomEventCheckFailed>>();

  clearRandomChanceResult(simulation);

  simulation.view<internal::removeAccuracyFromTargets>();
}

void internal::deductMoveHitCount(types::handle targetHandle, HitCount& hitCount) {
  ENTT_ASSERT(hitCount.val > 0, "A hit count shouldn't be decremented if it's already 0.");
  hitCount.val--;
  if (!hitCount.val) {
    targetHandle.remove<HitCount>();
  }
}

void moveHitLoop(Simulation& simulation) {
  setMoveHitCount(simulation);  // TODO(aed3): Handle each target one at a time

  while (!simulation.registry.view<HitCount>().empty()) {
    calc_damage::run(simulation);

    // for simulate turn only
    applyDamage(simulation);
    runSecondaryMoveEffects(simulation);
    runDamagingHitEvent(simulation);

    // Update stats if needed
    simulation.view<internal::deductMoveHitCount>();
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