#include "CalcDamage.hpp"

#include <Components/CalcDamage/Aliases.hpp>
#include <Components/CalcDamage/CalcDamageTarget.hpp>
#include <Components/CalcDamage/CriticalHit.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Selection.hpp>
#include <Components/RandomEventOutputs.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <SimulateTurn/RandomChance.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>
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

void internal::assignCritChanceDivisor(types::handle damageTargetHandle, CritBoost critBoost) {
  std::size_t index = std::min((std::size_t)critBoost.val, internal::CRIT_CHANCE_DIVISORS.size() - 1);
  damageTargetHandle.emplace<CritChanceDivisor>(internal::CRIT_CHANCE_DIVISORS[index]);
}

void setIfMoveCrits(Simulation& simulation) {
  simulation.addToEntities<CritBoost, tags::CalcDamageTarget>();
  runModifyCritBoostEvent(simulation);
  simulation.view<internal::assignCritChanceDivisor, Tags<tags::CalcDamageTarget>>();
  simulation.registry.clear<CritBoost>();

  setReciprocalRandomBinaryChoice<CritChanceDivisor, tags::CalcDamageTarget>(simulation);
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