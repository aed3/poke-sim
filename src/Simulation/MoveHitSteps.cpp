#include "MoveHitSteps.hpp"

#include <Battle/ManageBattleState.hpp>
#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <CalcDamage/CalcDamage.hpp>
#include <CalcDamage/Setup/CalcDamageInputSetup.hpp>
#include <Components/Accuracy.hpp>
#include <Components/BaseEffectChance.hpp>
#include <Components/Damage.hpp>
#include <Components/Effects/AddedFlinchChance.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/HitCount.hpp>
#include <Components/Priority.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Components/SimulateTurn/MoveHitStepTags.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MovePropertyTags.hpp>
#include <Components/Tags/VolatileTags.hpp>
#include <Config/Require.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Types/Constants.hpp>
#include <Types/Enums/BattleFormat.hpp>
#include <Types/Registry.hpp>
#include <Utilities/EntityFilter.hpp>
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
  if (simulation.registry.view<tags::CurrentActionMove>().empty()) {
    return;
  }

  simulation.addToEntities<tags::CurrentMoveHit, tags::CurrentActionMove>();

  Function(simulation);

  simulation.view<internal::setFailedActionMove, Tags<tags::FailedCurrentMoveHit>>();
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
  handle.registry()->emplace<CurrentEffectsAsTarget>(target.val, handle.entity());
}

void runMoveEffects(Simulation& simulation) {
  internal::EntityFilter<internal::tags::RunEffect> moveFilter{simulation};
  if (moveFilter.hasNoneSelected()) {
    return;
  }

  moveFilter.view<setEffectSource>();
  moveFilter.view<setEffectTarget<CurrentActionSource>, Tags<move::effect::tags::MoveSource>>();
  moveFilter.view<setEffectTarget<CurrentActionTarget>, Tags<move::effect::tags::MoveTarget>>();

  internal::tryBoost(simulation);
  internal::trySetStatus(simulation);
  trySetStatusFromEffect(simulation);
  trySetVolatileFromEffect(simulation);
  trySetSideConditionFromEffect(simulation);
  trySetSlotConditionFromEffect(simulation);
  trySetWeatherFromEffect(simulation);
  trySetTerrainFromEffect(simulation);
  trySetPseudoWeatherFromEffect(simulation);
  internal::runHitEvent(simulation);

  simulation.registry.clear<CurrentEffectSource, CurrentEffectTarget, CurrentEffectsAsSource, CurrentEffectsAsTarget>();
}

template <typename ComponentToRemove>
void removeImpossibleEffectTarget(
  types::handle handle, types::entity target, types::percentChance percentChance, Battle battle,
  const simulate_turn::Options& options) {
  types::registry& registry = *handle.registry();
  internal::PercentChanceLimitResult limitReached = internal::checkPercentChanceLimits(
    percentChance * Constants::PercentChanceToProbability,
    registry.get<Probability>(battle.val).val,
    options);

  if (limitReached == internal::PercentChanceLimitResult::REACHED_PASS_LIMIT) {
    return;
  }

  types::stat hp = registry.get<stat::CurrentHp>(target).val;
  if (hp == Constants::PokemonCurrentHpStat::MIN) {
    handle.remove<ComponentToRemove>();
  }
}

template <typename TargetEntityHolder>
void removeImpossibleSecondaryEffectTarget(
  types::handle handle, TargetEntityHolder target, BaseEffectChance baseEffectChance, Battle battle,
  const simulate_turn::Options& options) {
  removeImpossibleEffectTarget<move::effect::tags::Secondary>(
    handle,
    target.val,
    baseEffectChance.val,
    battle,
    options);
}

// Skipping secondary effects entirely for a fainted target is not something Showdown does. This is done here to prevent
// more random chance splits than needed and should not cause outcome deviations from Showdown. If, for example, a move
// exists that has a random chance to add a side or field effect regardless of the target's HP, then this function will
// need to be reworked.
void removeImpossibleSecondaryEffectTargets(Simulation& simulation) {
  internal::EntityFilter<move::effect::tags::Secondary> moveFilter{simulation};
  if (moveFilter.hasNoneSelected()) {
    return;
  }

  moveFilter.view<removeImpossibleSecondaryEffectTarget<CurrentActionSource>, Tags<move::effect::tags::MoveSource>>(
    simulation.simulateTurnOptions);
  moveFilter.view<removeImpossibleSecondaryEffectTarget<CurrentActionTarget>, Tags<move::effect::tags::MoveTarget>>(
    simulation.simulateTurnOptions);
}

void applyAddedFlinch(types::registry& registry, CurrentActionTarget target) {
  registry.emplace<tags::Flinch>(target.val);
}

void removeImpossibleAddedFlinch(
  types::handle handle, CurrentActionTarget target, AddedFlinchChance addedFlinchChance, Battle battle,
  const simulate_turn::Options& options) {
  types::registry& registry = *handle.registry();

  if (registry.all_of<tags::Flinch>(target.val)) {
    handle.remove<AddedFlinchChance>();
    return;
  }

  removeImpossibleEffectTarget<AddedFlinchChance>(handle, target.val, addedFlinchChance.val, battle, options);
}

void runAddedFlinchEffect(Simulation& simulation) {
  simulation.view<removeImpossibleAddedFlinch, Tags<tags::CurrentMoveHit>>(simulation.simulateTurnOptions);

  internal::runRandomBinaryChance<AddedFlinchChance, tags::CurrentMoveHit>(simulation, [](Simulation& sim) {
    sim.view<applyAddedFlinch, Tags<internal::tags::RandomEventCheckPassed>>();
  });
}

// TODO(aed3): When adding damage source, change this to accept the move's handle and CurrentActionSource to pass to
// applyDamage.
void applyDamageToTarget(types::registry& registry, Damage damage, CurrentActionTarget target) {
  internal::applyDamage({registry, target.val}, damage.val);
}

void setMoveHitCount(Simulation& simulation) {
  simulation.addToEntitiesWithExclude<HitCount, tags::CurrentMoveHit>(
    entt::exclude<move::tags::VariableHitCount, HitCount>,
    HitCount{Constants::MoveHits::DEFAULT});

  internal::runRandomEventChances<4U, tags::CurrentMoveHit, move::tags::VariableHitCount>(
    simulation,
    Constants::PROGRESSIVE_MULTI_HIT_CHANCES,
    [](Simulation& sim) {
      sim.addToEntities<HitCount, internal::tags::RandomEventA>(HitCount{2U});
      sim.addToEntities<HitCount, internal::tags::RandomEventB>(HitCount{3U});
      sim.addToEntities<HitCount, internal::tags::RandomEventC>(HitCount{4U});
      sim.addToEntities<HitCount, internal::tags::RandomEventD>(HitCount{5U});
    });
}

void applyDamage(Simulation& simulation) {
  simulation.view<applyDamageToTarget>();

  simulation.addToEntitiesWithExclude<tags::FailedCurrentMoveHit, tags::CurrentMoveHit>(
    entt::exclude<Damage, move::tags::Status>);
  simulation.removeFromEntities<tags::CurrentMoveHit, tags::FailedCurrentMoveHit>();
  simulation.removeFromEntities<Damage>();
}

void runPrimaryMoveEffects(Simulation& simulation) {
  simulation.addToEntities<internal::tags::RunEffect, move::effect::tags::Primary, tags::CurrentMoveHit>();
  runMoveEffects(simulation);
  simulation.removeFromEntities<internal::tags::RunEffect>();
}

void setSourceSwitchFlag(types::registry& registry, CurrentActionSource source, const Simulation& simulation) {
  if (internal::sideHasPossibleSwitch(registry, registry.get<Side>(source.val), simulation)) {
    registry.emplace<tags::Switching>(source.val);
  }
}

void runSelfSwitch(Simulation& simulation) {
  simulation.view<setSourceSwitchFlag, Tags<move::tags::SelfSwitch, tags::CurrentMoveHit>>(simulation);
}

void runSecondaryMoveEffects(Simulation& simulation) {
  removeImpossibleSecondaryEffectTargets(simulation);
  internal::runModifySecondariesEvent(simulation);

  internal::runRandomBinaryChance<BaseEffectChance, move::effect::tags::Secondary, tags::CurrentMoveHit>(
    simulation,
    [](Simulation& sim) { sim.addToEntities<internal::tags::RunEffect, internal::tags::RandomEventCheckPassed>(); });

  runMoveEffects(simulation);
  simulation.removeFromEntities<internal::tags::RunEffect>();

  runAddedFlinchEffect(simulation);
}

void accuracyCheck(Simulation& simulation) {
  internal::runModifyAccuracyEvent(simulation);
  internal::runAccuracyEvent(simulation);

  internal::runRandomBinaryChance<Accuracy, tags::CurrentMoveHit>(simulation, [](Simulation& sim) {
    sim.addToEntities<tags::FailedCurrentMoveHit, internal::tags::RandomEventCheckFailed>();
  });
}

void moveHitLoop(Simulation& simulation) {
  setMoveHitCount(simulation);

  using MoveHitLimits = Constants::MoveHits;
  types::moveHits iterations = MoveHitLimits::MIN;
  while (!simulation.registry.view<HitCount>().empty()) {
    POKESIM_REQUIRE(iterations <= MoveHitLimits::MAX, "More hits were ran than possible.");

    calc_damage::run(simulation);  // 1. call to this.battle.getDamage
    internal::runDamageEvent(simulation);

    applyDamage(simulation);            // 2. call to this.battle.spreadDamage
    runPrimaryMoveEffects(simulation);  // 3. primary effects
    runSelfSwitch(simulation);
    // 4. self drops
    runSecondaryMoveEffects(simulation);  // 5. secondary effects
    // 6. force switch

    internal::runDamagingHitEvent(simulation);
    internal::runAfterHitEvent(simulation);

    internal::updateAllStats(simulation);
    simulation.view<deductMoveHitCount>();
    simulation.view<removeHitCountFromFaintedTargets, Tags<tags::CurrentActionMove>>();
    iterations++;
  }
}
}  // namespace

void internal::runMoveHitChecks(Simulation& simulation) {
  // invulnerabilityCheck
  // hitCheck
  // immunityCheck
  runMoveHitCheck<accuracyCheck>(simulation);
  // breakProtectCheck
  // stealBoostCheck
  runMoveHitCheck<moveHitLoop>(simulation);
}
}  // namespace pokesim
