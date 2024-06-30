#include "CalcDamage.hpp"

#include <Components/CalcDamage/Aliases.hpp>
#include <Components/CalcDamage/CriticalHit.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Components/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Simulation/RunEvent.hpp>
#include <Types/Registry.hpp>
#include <Simulation/Simulation.hpp>
#include <Utilities/SelectForView.hpp>
#include <cmath>

namespace pokesim::calc_damage {
void run(Simulation& simulation) {
  getDamage(simulation);

  clearRunVariables(simulation);
}

void clearRunVariables(Simulation& simulation) {
  simulation.registry.clear<Crit>();
}

void modifyDamageWithTypes(Simulation& /*simulation*/) {}

void getDamageRole(Simulation& /*simulation*/) {}

void internal::assignCritChanceDivisor(types::handle moveHandle, CritBoost critBoost) {
  std::size_t index = std::min((std::size_t)critBoost.val, internal::CRIT_CHANCE_DIVISORS.size() - 1);
  moveHandle.emplace<CritChanceDivisor>(internal::CRIT_CHANCE_DIVISORS[index]);
}

void setIfMoveCrits(Simulation& simulation) {
  pokesim::internal::SelectForCurrentActionMoveView<pokesim::tags::SimulateTurn> selectedMoves{simulation};
  if (selectedMoves.hasNoneSelected()) return;

  simulation.addToEntities<CritBoost, SelectedForViewMove>();
  runModifyCritBoostEvent(simulation);
  simulation.viewForSelectedMoves<internal::assignCritChanceDivisor>();
  simulation.registry.clear<CritBoost>();

  setReciprocalRandomBinaryChoice<CritChanceDivisor, SelectedForViewMove>(simulation);
  reciprocalRandomBinaryChance(simulation, [](Simulation& sim) {
    sim.addToEntities<Crit, pokesim::tags::RandomEventCheckPassed>();
  });

  clearRandomChanceResult(simulation);
  simulation.registry.clear<CritChanceDivisor>();
}

void getDamage(Simulation& simulation) {
  setIfMoveCrits(simulation);

  // Get base power, boosts, get atk/def stats
  runBasePowerEvent(simulation);

  getDamageRole(simulation);

  modifyDamageWithTypes(simulation);
}
}  // namespace pokesim::calc_damage