#include "MoveHitSteps.hpp"

#include <Battle/Pokemon/ManagePokemonState.hpp>
#include <CalcDamage/CalcDamage.hpp>
#include <CalcDamage/Setup/CalcDamageInputSetup.hpp>
#include <Components/Accuracy.hpp>
#include <Components/Damage.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/HitCount.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Components/SimulateTurn/MoveHitStepTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/MoveTags.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Types/Enums/BattleFormat.hpp>
#include <Types/Registry.hpp>
#include <Utilities/SelectForView.hpp>
#include <Utilities/Tags.hpp>
#include <algorithm>

#include "RunEvent.hpp"
#include "Simulation.hpp"

namespace pokesim {
void setMoveHitCount(Simulation& simulation) {
  auto noAssignedHitCount =
    simulation.registry.view<tags::SelectedForViewMove>(entt::exclude<move::tags::VariableHitCount, HitCount>);
  simulation.registry.insert<HitCount>(noAssignedHitCount.begin(), noAssignedHitCount.end(), {(types::moveHits)1U});

  // The 35%-35%-15%-15% out of 100 for 2-3-4-5 hits added so each index is the sum of the chance of its hit count and
  // the hit counts less than it so it works with the randomEventChances function
  static constexpr std::array<types::percentChance, 4U> progressiveMultiHitChances{35U, 70U, 85U, 100U};
  setRandomChoice<4U, tags::SelectedForViewMove, move::tags::VariableHitCount>(
    simulation,
    progressiveMultiHitChances,
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

  simulation.removeFromEntities<tags::internal::MoveHits, tags::SelectedForViewMove>(entt::exclude<Damage>);
  simulation.removeFromEntities<tags::SelectedForViewMove, tags::SelectedForViewMove>(
    entt::exclude<tags::internal::MoveHits>);
}

void trySetStatusFromEffect(Simulation& /*simulation*/) {}

void runSecondaryMoveEffects(Simulation& simulation) {
  // Set secondary effect of active move

  trySetStatusFromEffect(simulation);
}

void accuracyCheck(Simulation& simulation) {
  runModifyAccuracyEvent(simulation);
  runAccuracyEvent(simulation);

  setRandomBinaryChoice<Accuracy, tags::SelectedForViewMove>(simulation);
  randomBinaryChance(simulation, [](Simulation& sim) {
    sim.removeFromEntities<tags::internal::MoveHits, tags::SelectedForViewMove, tags::RandomEventCheckFailed>();
  });
}

void internal::deductMoveHitCount(types::handle moveHandle, HitCount& hitCount) {
  ENTT_ASSERT(hitCount.val > 0, "A hit count shouldn't be decremented if it's already 0.");
  hitCount.val--;
  if (!hitCount.val) {
    moveHandle.remove<HitCount>();
  }
}

void moveHitLoop(Simulation& simulation) {
  setMoveHitCount(simulation);

  while (!simulation.registry.view<HitCount>().empty()) {
    internal::SelectForCurrentActionMoveView<HitCount> selectedMoves{simulation};
    calc_damage::run(simulation);

    // for simulate turn only
    applyDamage(simulation);
    runSecondaryMoveEffects(simulation);
    runDamagingHitEvent(simulation);

    updateAllStats(simulation);
    internal::postMoveHitCheck(simulation);
    simulation.view<internal::deductMoveHitCount>();
  }
}

void internal::removeFailedHitTargets(
  types::handle moveTarget, const CurrentActionTargets& targets, const CurrentActionSource& source) {
  types::registry& registry = *moveTarget.registry();
  for (types::entity target : targets.val) {
    registry.remove<tags::CurrentActionMoveTarget>(target);

    CurrentActionMoves& moves = registry.get<CurrentActionMoves>(target);
    auto newEnd = std::remove(moves.val.begin(), moves.val.end(), moveTarget.entity());
    moves.val.erase(newEnd, moves.val.end());
  }

  CurrentActionMoves& moves = registry.get<CurrentActionMoves>(source.val);
  auto newEnd = std::remove(moves.val.begin(), moves.val.end(), moveTarget.entity());
  moves.val.erase(newEnd, moves.val.end());
}

void internal::updateCurrentActionTargets(types::registry& registry, CurrentActionTargets& targets) {
  std::uint8_t deleteCount = 0U;
  for (types::entity& target : targets.val) {
    if (!registry.all_of<tags::CurrentActionMoveTarget>(target)) {
      std::uint8_t swapIndex = targets.val.size() - 1 - deleteCount;
      ENTT_ASSERT(swapIndex >= 0 && swapIndex < targets.val.size(), "Swap index out of bounds");
      std::swap(target, targets.val[swapIndex]);
      deleteCount++;
    }
  }

  targets.val.pop_count(deleteCount);
}

void internal::postMoveHitCheck(Simulation& simulation) {
  auto removedMoves = simulation.view<
    internal::removeFailedHitTargets,
    Tags<tags::CurrentActionMove>,
    entt::exclude_t<tags::internal::MoveHits>>();
  simulation.registry.destroy(removedMoves.begin(), removedMoves.end());
  simulation.registry.clear<Damage, DamageRolls>();
  simulation.view<internal::updateCurrentActionTargets>();
}

template <auto Function>
void internal::runMoveHitCheck(Simulation& simulation) {
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

void runMoveHitChecks(Simulation& simulation) {
  // invulnerabilityCheck
  // hitCheck
  // immunityCheck
  internal::runMoveHitCheck<accuracyCheck>(simulation);
  // breakProtectCheck
  // stealBoostCheck
  internal::runMoveHitCheck<moveHitLoop>(simulation);
}
}  // namespace pokesim