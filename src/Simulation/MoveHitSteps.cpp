#include "MoveHitSteps.hpp"

#include <Battle/ManageBattleState.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <CalcDamage/CalcDamage.hpp>
#include <CalcDamage/Setup/CalcDamageInputSetup.hpp>
#include <Components/Accuracy.hpp>
#include <Components/BaseEffectChance.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/HitCount.hpp>
#include <Components/Priority.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Components/SimulateTurn/MoveHitStepTags.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Config/Require.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Types/Constants.hpp>
#include <Types/Enums/BattleFormat.hpp>
#include <Types/Registry.hpp>
#include <Utilities/SelectForView.hpp>
#include <Utilities/Tags.hpp>

#include "RunEvent.hpp"
#include "Simulation.hpp"

namespace pokesim {
namespace {
void deductMoveHitCount(types::handle moveHandle, HitCount& hitCount) {
  POKESIM_REQUIRE(hitCount.val > 0U, "A hit count shouldn't be decremented if it's already 0.");
  hitCount.val--;
  if (!hitCount.val) {
    moveHandle.remove<HitCount, tags::CurrentMoveHit>();
  }
}

void removeHitCountFromFaintedTargets(types::handle moveHandle, CurrentActionTarget target) {
  if (moveHandle.registry()->get<stat::CurrentHp>(target.val).val == Constants::PokemonCurrentHpStat::MIN) {
    moveHandle.remove<HitCount, tags::CurrentMoveHit>();
  }
}

template <auto Function>
void runMoveHitCheck(Simulation& simulation) {
  simulation.addToEntities<tags::CurrentMoveHit, tags::CurrentActionMove>();

  internal::SelectForCurrentActionMoveView<tags::CurrentMoveHit> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) {
    return;
  }

  Function(simulation);
  selectedMoves.deselect();

  simulation.view<setFailedActionMove, Tags<tags::FailedCurrentMoveHit>>();
  simulation.registry.clear<tags::CurrentMoveHit, tags::FailedCurrentMoveHit>();
}

void trySetStatusFromEffect(Simulation&) {}
void trySetVolatileFromEffect(Simulation&) {}
void trySetSideConditionFromEffect(Simulation&) {}
void trySetSlotConditionFromEffect(Simulation&) {}
void trySetWeatherFromEffect(Simulation&) {}
void trySetTerrainFromEffect(Simulation&) {}
void trySetPseudoWeatherFromEffect(Simulation&) {}

void setEffectSource(types::handle handle, CurrentActionSource source) {
  handle.emplace<CurrentEffectSource>(source.val);
  handle.registry()->get_or_emplace<CurrentEffectsAsSource>(source.val).val.push_back(handle.entity());
}

template <typename TargetEntityHolder>
void setEffectTarget(types::handle handle, TargetEntityHolder target) {
  handle.emplace<CurrentEffectTarget>(target.val);
  handle.registry()->get_or_emplace<CurrentEffectsAsTarget>(target.val).val.push_back(handle.entity());
}

void runMoveEffects(Simulation& simulation) {
  internal::SelectForCurrentActionMoveView<internal::tags::RunEffect> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) {
    return;
  }

  simulation.viewForSelectedMoves<setEffectSource>();
  simulation.viewForSelectedMoves<setEffectTarget<CurrentActionSource>, Tags<move::effect::tags::MoveSource>>();
  simulation.viewForSelectedMoves<setEffectTarget<CurrentActionTarget>, Tags<move::effect::tags::MoveTarget>>();

  tryBoost(simulation);
  trySetStatus(simulation);
  trySetStatusFromEffect(simulation);
  trySetVolatileFromEffect(simulation);
  trySetSideConditionFromEffect(simulation);
  trySetSlotConditionFromEffect(simulation);
  trySetWeatherFromEffect(simulation);
  trySetTerrainFromEffect(simulation);
  trySetPseudoWeatherFromEffect(simulation);

  simulation.registry.clear<CurrentEffectSource, CurrentEffectTarget, CurrentEffectsAsSource, CurrentEffectsAsTarget>();
}

template <typename TargetEntityHolder>
void removeFaintedSecondaryEffectTarget(
  types::handle handle, TargetEntityHolder target, BaseEffectChance baseEffectChance, Battle battle,
  const simulate_turn::Options& options) {
  types::registry& registry = *handle.registry();
  internal::PercentChanceLimitResult limitReached = internal::checkPercentChanceLimits(
    baseEffectChance.val * Constants::PercentChanceToProbability,
    registry.get<Probability>(battle.val).val,
    options);

  if (limitReached == internal::PercentChanceLimitResult::REACHED_PASS_LIMIT) {
    return;
  }
  types::stat hp = registry.get<stat::CurrentHp>(target.val).val;
  if (hp == Constants::PokemonCurrentHpStat::MIN) {
    handle.remove<move::effect::tags::Secondary>();
  }
}

// Skipping secondary effects entirely for a fainted target is not something Showdown does. This is done here to prevent
// more random chance splits than needed and should not cause outcome deviations from Showdown. If, for example, a move
// exists that has a random chance to add a side or field affect regardless of the target's HP, then this function will
// need to be reworked.
void removeFaintedSecondaryEffectTargets(Simulation& simulation) {
  internal::SelectForCurrentActionMoveView<move::effect::tags::Secondary> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) {
    return;
  }

  simulation.viewForSelectedMoves<
    removeFaintedSecondaryEffectTarget<CurrentActionSource>,
    Tags<move::effect::tags::MoveSource>>(simulation.simulateTurnOptions);
  simulation.viewForSelectedMoves<
    removeFaintedSecondaryEffectTarget<CurrentActionTarget>,
    Tags<move::effect::tags::MoveTarget>>(simulation.simulateTurnOptions);
}

// TODO(aed3): When adding damage source, change this to accept the move's handle and CurrentActionSource to pass to
// applyDamage.
void applyDamageToTarget(types::registry& registry, Damage damage, CurrentActionTarget target) {
  pokesim::applyDamage({registry, target.val}, damage.val);
}

void setMoveHitCount(Simulation& simulation) {
  auto noAssignedHitCount =
    simulation.registry.view<tags::CurrentMoveHit>(entt::exclude<move::tags::VariableHitCount, HitCount>);
  simulation.registry.insert<HitCount>(noAssignedHitCount.begin(), noAssignedHitCount.end(), {(types::moveHits)1U});

  runRandomEventChances<4U, tags::CurrentMoveHit, move::tags::VariableHitCount>(
    simulation,
    Constants::PROGRESSIVE_MULTI_HIT_CHANCES,
    [](Simulation& sim) {
      sim.addToEntities<HitCount, pokesim::internal::tags::RandomEventA>(HitCount{2U});
      sim.addToEntities<HitCount, pokesim::internal::tags::RandomEventB>(HitCount{3U});
      sim.addToEntities<HitCount, pokesim::internal::tags::RandomEventC>(HitCount{4U});
      sim.addToEntities<HitCount, pokesim::internal::tags::RandomEventD>(HitCount{5U});
    });
}

void applyDamage(Simulation& simulation) {
  simulation.viewForSelectedMoves<applyDamageToTarget>();

  auto view = simulation.registry.view<tags::CurrentMoveHit>(entt::exclude<Damage, move::tags::Status>);
  simulation.registry.insert<tags::FailedCurrentMoveHit>(view.begin(), view.end());
  simulation.removeFromEntities<tags::CurrentMoveHit, tags::FailedCurrentMoveHit>();
  simulation.registry.clear<Damage>();
}

void runPrimaryMoveEffects(Simulation& simulation) {
  simulation.addToEntities<internal::tags::RunEffect, move::effect::tags::Primary, tags::CurrentMoveHit>();
  runMoveEffects(simulation);
  simulation.registry.clear<internal::tags::RunEffect>();
}

void runSecondaryMoveEffects(Simulation& simulation) {
  removeFaintedSecondaryEffectTargets(simulation);
  runModifySecondariesEvent(simulation);

  runRandomBinaryChance<BaseEffectChance, move::effect::tags::Secondary, tags::CurrentMoveHit>(
    simulation,
    [](Simulation& sim) {
      sim.addToEntities<internal::tags::RunEffect, pokesim::internal::tags::RandomEventCheckPassed>();
    });

  runMoveEffects(simulation);
  simulation.registry.clear<internal::tags::RunEffect>();
}

void accuracyCheck(Simulation& simulation) {
  runModifyAccuracyEvent(simulation);
  runAccuracyEvent(simulation);

  runRandomBinaryChance<Accuracy, tags::CurrentMoveHit>(simulation, [](Simulation& sim) {
    sim.addToEntities<tags::FailedCurrentMoveHit, pokesim::internal::tags::RandomEventCheckFailed>();
  });
}

void moveHitLoop(Simulation& simulation) {
  setMoveHitCount(simulation);

  using MoveHitLimits = Constants::MoveHits;
  types::moveHits iterations = MoveHitLimits::MIN;
  while (!simulation.registry.view<HitCount>().empty()) {
    POKESIM_REQUIRE(iterations <= MoveHitLimits::MAX, "More hits were ran more than possible.");

    internal::SelectForCurrentActionMoveView<HitCount> selectedMoves{simulation};
    POKESIM_REQUIRE(
      !selectedMoves.hasNoneSelected(),
      "HitCount should only be present on active moves, meaning this loop should only be entered if there are moves to "
      "select.");
    calc_damage::run(simulation);  // 1. call to this.battle.getDamage
    runDamageEvent(simulation);

    applyDamage(simulation);            // 2. call to this.battle.spreadDamage
    runPrimaryMoveEffects(simulation);  // 3. primary effects
    // 4. self drops
    runSecondaryMoveEffects(simulation);  // 5. secondary effects
    // 6. force switch

    runDamagingHitEvent(simulation);
    runAfterHitEvent(simulation);

    updateAllStats(simulation);
    simulation.view<deductMoveHitCount>();
    simulation.view<removeHitCountFromFaintedTargets, Tags<tags::CurrentActionMove>>();
    iterations++;
  }
}
}  // namespace

void runMoveHitChecks(Simulation& simulation) {
  // invulnerabilityCheck
  // hitCheck
  // immunityCheck
  runMoveHitCheck<accuracyCheck>(simulation);
  // breakProtectCheck
  // stealBoostCheck
  runMoveHitCheck<moveHitLoop>(simulation);
}
}  // namespace pokesim
