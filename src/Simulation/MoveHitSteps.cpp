#include "MoveHitSteps.hpp"

#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <CalcDamage/CalcDamage.hpp>
#include <CalcDamage/Setup/CalcDamageInputSetup.hpp>
#include <Components/Accuracy.hpp>
#include <Components/BaseEffectChance.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/HitCount.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Components/SimulateTurn/MoveHitStepTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MoveTags.hpp>
#include <Config/Require.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Types/Enums/BattleFormat.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Registry.hpp>
#include <Utilities/SelectForView.hpp>
#include <Utilities/Tags.hpp>
#include <algorithm>

#include "RunEvent.hpp"
#include "Simulation.hpp"

namespace pokesim {
namespace {
void deductMoveHitCount(types::handle moveHandle, HitCount& hitCount) {
  POKESIM_REQUIRE(hitCount.val > 0U, "A hit count shouldn't be decremented if it's already 0.");
  hitCount.val--;
  if (!hitCount.val) {
    moveHandle.remove<HitCount>();
  }
}

void updateCurrentActionTargets(types::registry& registry, CurrentActionTargets& targets) {
  types::activePokemonIndex deleteCount = 0U;
  for (types::entity& target : targets.val) {
    if (!registry.all_of<tags::CurrentActionMoveTarget>(target)) {
      types::activePokemonIndex swapIndex = targets.val.size() - 1 - deleteCount;
      POKESIM_REQUIRE(swapIndex < targets.val.size(), "Swap index out of bounds.");
      std::swap(target, targets.val[swapIndex]);
      deleteCount++;
    }
  }

  targets.val.pop_count(deleteCount);
}

void removeFailedHitTargets(types::handle moveHandle, CurrentActionTarget target, CurrentActionSource source) {
  types::registry& registry = *moveHandle.registry();

  registry.remove<tags::CurrentActionMoveTarget>(target.val);

  CurrentActionMovesAsTarget& targetedMoves = registry.get<CurrentActionMovesAsTarget>(target.val);
  auto newTargetedMoveEnd = std::remove(targetedMoves.val.begin(), targetedMoves.val.end(), moveHandle.entity());
  targetedMoves.val.erase(newTargetedMoveEnd, targetedMoves.val.end());

  CurrentActionMovesAsSource& sourcedMoves = registry.get<CurrentActionMovesAsSource>(source.val);
  auto newSourcedMoveEnd = std::remove(sourcedMoves.val.begin(), sourcedMoves.val.end(), moveHandle.entity());
  sourcedMoves.val.erase(newSourcedMoveEnd, sourcedMoves.val.end());
}

void postMoveHitCheck(Simulation& simulation) {
  auto removedMoves =
    simulation.view<removeFailedHitTargets, Tags<tags::CurrentActionMove>, entt::exclude_t<tags::internal::MoveHits>>();
  simulation.registry.destroy(removedMoves.begin(), removedMoves.end());
  simulation.registry.clear<Damage>();
  simulation.view<updateCurrentActionTargets>();
}

template <auto Function>
void runMoveHitCheck(Simulation& simulation) {
  simulation.addToEntities<tags::internal::MoveHits, tags::CurrentActionMove>();

  internal::SelectForCurrentActionMoveView<tags::internal::MoveHits> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) {
    return;
  }

  Function(simulation);
  selectedMoves.deselect();

  postMoveHitCheck(simulation);
  simulation.registry.clear<tags::internal::MoveHits>();
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
  internal::SelectForCurrentActionMoveView<tags::internal::RunEffect> selectedMoves{simulation};
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
}  // namespace

void setMoveHitCount(Simulation& simulation) {
  auto noAssignedHitCount =
    simulation.registry.view<tags::SelectedForViewMove>(entt::exclude<move::tags::VariableHitCount, HitCount>);
  simulation.registry.insert<HitCount>(noAssignedHitCount.begin(), noAssignedHitCount.end(), {(types::moveHits)1U});

  setRandomChoice<4U, tags::SelectedForViewMove, move::tags::VariableHitCount>(
    simulation,
    MechanicConstants::PROGRESSIVE_MULTI_HIT_CHANCES,
    false);

  if (!simulation.registry.view<RandomEventChances<4U>>().empty()) {
    randomEventChances<4U>(simulation, [](Simulation& sim) {
      sim.addToEntities<HitCount, tags::RandomEventA>(HitCount{2U});
      sim.addToEntities<HitCount, tags::RandomEventB>(HitCount{3U});
      sim.addToEntities<HitCount, tags::RandomEventC>(HitCount{4U});
      sim.addToEntities<HitCount, tags::RandomEventD>(HitCount{5U});
    });
  }
}

void applyDamage(Simulation& simulation) {
  simulation.viewForSelectedMoves<applyDamageToHp>();

  simulation.removeFromEntities<tags::internal::MoveHits, tags::SelectedForViewMove>(
    entt::exclude<Damage, move::tags::Status>);
  simulation.removeFromEntities<tags::SelectedForViewMove>(entt::exclude<tags::internal::MoveHits>);
}

void runPrimaryMoveEffects(Simulation& simulation) {
  simulation.addToEntities<tags::internal::RunEffect, move::effect::tags::Primary, tags::SelectedForViewMove>();
  runMoveEffects(simulation);
  simulation.registry.clear<tags::internal::RunEffect>();
}

void runSecondaryMoveEffects(Simulation& simulation) {
  runModifySecondariesEvent(simulation);

  setRandomBinaryChoice<BaseEffectChance, move::effect::tags::Secondary, tags::SelectedForViewMove>(simulation);
  randomBinaryChance(simulation, [](Simulation& sim) {
    sim.addToEntities<tags::internal::RunEffect, tags::SelectedForViewMove, tags::RandomEventCheckPassed>();
  });
  runMoveEffects(simulation);
  simulation.registry.clear<tags::internal::RunEffect>();
}

void accuracyCheck(Simulation& simulation) {
  runModifyAccuracyEvent(simulation);
  runAccuracyEvent(simulation);

  setRandomBinaryChoice<Accuracy, tags::SelectedForViewMove>(simulation);
  randomBinaryChance(simulation, [](Simulation& sim) {
    sim.removeFromEntities<tags::internal::MoveHits, tags::SelectedForViewMove, tags::RandomEventCheckFailed>();
  });
}

void moveHitLoop(Simulation& simulation) {
  setMoveHitCount(simulation);

  while (!simulation.registry.view<HitCount>().empty()) {
    internal::SelectForCurrentActionMoveView<HitCount> selectedMoves{simulation};
    POKESIM_REQUIRE(
      !selectedMoves.hasNoneSelected(),
      "HitCount should only be present on active moves, meaning this loop should only be entered if there are moves to "
      "select.");
    calc_damage::run(simulation);  // 1. call to this.battle.getDamage

    applyDamage(simulation);            // 2. call to this.battle.spreadDamage
    runPrimaryMoveEffects(simulation);  // 3. primary effects
    // 4. self drops
    runSecondaryMoveEffects(simulation);  // 5. secondary effects
    // 6. force switch

    runDamagingHitEvent(simulation);

    updateAllStats(simulation);
    postMoveHitCheck(simulation);
    simulation.view<deductMoveHitCount>();
  }
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
}  // namespace pokesim