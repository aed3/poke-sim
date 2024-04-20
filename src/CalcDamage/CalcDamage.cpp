#include "CalcDamage.hpp"

#include <Simulation/RandomChance.hpp>
#include <Simulation/RunEvent.hpp>
#include <Simulation/Simulation.hpp>

namespace pokesim::calc_damage {
void run(Simulation& simulation) {}

void criticalHitRandomChance(Simulation& simulation) {
  // Set critical hit chances as random chance variable

  randomChance(simulation);
}

void modifyDamageWithTypes(Simulation& simulation) {}

void getDamageRole(Simulation& simulation) {
  sampleRandomChance(simulation);
}

void getCritMultiplier(Simulation& simulation) {
  runModifyCritRatioEvent(simulation);
}

void getDamage(Simulation& simulation) {
  getCritMultiplier(simulation);
  criticalHitRandomChance(simulation);

  // Get base power, boosts, get atk/def stats
  runBasePowerEvent(simulation);

  getDamageRole(simulation);

  modifyDamageWithTypes(simulation);
}
}  // namespace pokesim::calc_damage