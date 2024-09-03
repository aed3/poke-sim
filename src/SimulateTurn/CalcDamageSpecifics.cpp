#include "CalcDamageSpecifics.hpp"

#include <CalcDamage/CalcDamage.hpp>
#include <CalcDamage/Helpers.hpp>
#include <Components/CalcDamage/Aliases.hpp>
#include <Components/CalcDamage/CriticalHit.hpp>
#include <Components/Damage.hpp>
#include <Components/Probability.hpp>
#include <Components/RandomEventInputs.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Damage.hpp>
#include <Types/Enums/DamageRollKind.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Random.hpp>
#include <Utilities/SelectForView.hpp>

namespace pokesim::simulate_turn {
namespace internal {
void applyDamageRollIndex(Damage& damage, const DamageRolls& damageRolls, const RandomEventIndex& randomRollIndex) {
  types::eventPossibilities damageRollIndex = 0U;
  for (std::size_t i = 0U; i < damageRolls.val.size(); i++) {
    if (randomRollIndex.val == damageRollIndex) {
      damage = damageRolls.val[damageRollIndex];
      return;
    }

    damageRollIndex += i == 0 || damageRolls.val[i - 1].val != damageRolls.val[i].val ? 1 : 0;
  }

  ENTT_FAIL("How was a damage roll not found that matched the event index?");
}

void assignProbability(types::registry& registry, const Battle& battle, const RandomEventCount& randomEventCount) {
  if (randomEventCount.val != 1U) {
    Probability& probability = registry.get<Probability>(battle.val);
    probability.val *= 1.0F / (types::probability)MechanicConstants::MAX_DAMAGE_ROLL_COUNT;
  }
}

void assignAllDamageRollProbability(
  types::registry& registry, const Damage& damage, DamageRolls& damageRolls, const Battle& battle,
  const RandomEventIndex& randomRollIndex) {
  types::eventPossibilities damageCount = 0U;
  for (const Damage damageRoll : damageRolls.val) {
    damageCount += damageRoll.val == damage.val ? 1 : 0;
  }

  ENTT_ASSERT(damageCount > 0U, "How was a damage roll not found that matched the damage dealt?");

  Probability& probability = registry.get<Probability>(battle.val);
  probability.val *= damageCount / (types::probability)MechanicConstants::MAX_DAMAGE_ROLL_COUNT;
}

types::eventPossibilities countUniqueDamageRolls(types::handle moveHandle) {
  const DamageRolls& damageRolls = moveHandle.get<DamageRolls>();
  types::eventPossibilities eventPossibilities = 1U;
  for (std::size_t i = 1U; i < damageRolls.val.size(); i++) {
    eventPossibilities += damageRolls.val[i - 1].val != damageRolls.val[i].val ? 1 : 0;
  }
  return eventPossibilities;
}
}  // namespace internal

void cloneFromDamageRolls(Simulation& simulation, DamageRollKind damageRollKind) {
  pokesim::internal::SelectForCurrentActionMoveView<pokesim::tags::SimulateTurn, DamageRolls> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) return;

  setRandomEventCounts<internal::countUniqueDamageRolls, pokesim::tags::SelectedForViewMove>(simulation);
  auto applyChoices = [](Simulation& sim) { sim.viewForSelectedMoves<internal::applyDamageRollIndex>(); };

  auto updateProbabilities =
    calc_damage::damageKindsMatch(damageRollKind, DamageRollKind::ALL_DAMAGE_ROLLS)
      ? [](Simulation& sim) { sim.viewForSelectedMoves<internal::assignAllDamageRollProbability>(); }
      : [](Simulation& sim) { sim.viewForSelectedMoves<internal::assignProbability>(); };

  randomEventCount(simulation, applyChoices, updateProbabilities);
  simulation.removeFromEntities<DamageRolls, pokesim::tags::SelectedForViewMove>();
}

void setIfMoveCrits(Simulation& simulation) {
  pokesim::internal::SelectForCurrentActionMoveView<pokesim::tags::SimulateTurn> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) return;

  setReciprocalRandomBinaryChoice<calc_damage::CritChanceDivisor, pokesim::tags::SelectedForViewMove>(simulation);
  reciprocalRandomBinaryChance(simulation, [](Simulation& sim) {
    sim.addToEntities<calc_damage::tags::Crit, pokesim::tags::RandomEventCheckPassed>();
  });

  simulation.registry.clear<calc_damage::CritChanceDivisor>();
}
}  // namespace pokesim::simulate_turn