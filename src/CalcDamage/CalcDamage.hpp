#pragma once

namespace pokesim {
class Simulation;

namespace calc_damage {
void run(Simulation& simulation);

void modifyDamageWithTypes(Simulation& simulation);
void getDamageRole(Simulation& simulation);

void criticalHitRandomChance(Simulation& simulation);
void getCritMultiplier(Simulation& simulation);
void getDamage(Simulation& simulation);
}  // namespace calc_damage
}  // namespace pokesim