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
#include <Config/Require.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Damage.hpp>
#include <Types/Enums/DamageRollKind.hpp>
#include <Types/MechanicConstants.hpp>
#include <Types/Random.hpp>
#include <Utilities/SelectForView.hpp>

namespace pokesim::simulate_turn {
namespace {
void applyDamageRollIndex(Damage& damage, const DamageRolls& damageRolls, const RandomEventIndex& randomRollIndex) {
  types::eventPossibilities damageRollIndex = 0U;
  for (types::damageRollIndex i = 0U; i < damageRolls.val.size(); i++) {
    if (randomRollIndex.val == damageRollIndex) {
      damage = damageRolls.val[damageRollIndex];
      return;
    }

    damageRollIndex += i == 0 || damageRolls.val[i - 1].val != damageRolls.val[i].val ? 1 : 0;
  }

  POKESIM_REQUIRE_FAIL("How was a damage roll not found that matched the event index?");
}

void assignPartialProbability(
  types::registry& registry, const Battle& battle, const RandomEventCount& randomEventCount) {
  if (randomEventCount.val != 1U) {
    Probability& probability = registry.get<Probability>(battle.val);
    probability.val *= 1.0F / (types::probability)randomEventCount.val;
  }
}

void assignAllDamageRollProbability(
  types::registry& registry, const Damage& damage, DamageRolls& damageRolls, const Battle& battle) {
  types::eventPossibilities damageCount = 0U;
  for (const Damage damageRoll : damageRolls.val) {
    damageCount += damageRoll.val == damage.val ? 1 : 0;
  }

  POKESIM_REQUIRE(damageCount > 0U, "How was a damage roll not found that matched the damage dealt?");

  Probability& probability = registry.get<Probability>(battle.val);
  probability.val *= damageCount / (types::probability)MechanicConstants::DamageRollCount::MAX;
}

types::eventPossibilities countUniqueDamageRolls(types::handle moveHandle) {
  const DamageRolls& damageRolls = moveHandle.get<DamageRolls>();
  types::eventPossibilities eventPossibilities = 1U;
  for (types::damageRollIndex i = 1U; i < damageRolls.val.size(); i++) {
    eventPossibilities += damageRolls.val[i - 1].val != damageRolls.val[i].val ? 1 : 0;
  }
  return eventPossibilities;
}

void updateAllDamageRollProbabilities(Simulation& simulation) {
  simulation.viewForSelectedMoves<assignAllDamageRollProbability>();
}

void updatePartialProbabilities(Simulation& simulation) {
  simulation.viewForSelectedMoves<assignPartialProbability>();
}
}  // namespace

void cloneFromDamageRolls(Simulation& simulation, DamageRollKind damageRollKind) {
  pokesim::internal::SelectForCurrentActionMoveView<pokesim::tags::SimulateTurn, DamageRolls> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) return;

  bool forAllDamageRolls = calc_damage::damageKindsMatch(damageRollKind, DamageRollKind::ALL_DAMAGE_ROLLS);
  bool forRequiredDamageRolls = simulation.simulateTurnOptions.makeBranchesOnRandomEvents || forAllDamageRolls;
  setRandomEventCounts<countUniqueDamageRolls, pokesim::tags::SelectedForViewMove>(simulation, forRequiredDamageRolls);

  auto applyChoices = [](Simulation& sim) { sim.viewForSelectedMoves<applyDamageRollIndex>(); };

  auto updateProbabilities = forAllDamageRolls ? updateAllDamageRollProbabilities : updatePartialProbabilities;

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