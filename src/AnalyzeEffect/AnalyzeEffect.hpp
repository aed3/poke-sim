#pragma once

namespace pokesim {
class Simulation;

namespace analyze_effect {
void run(Simulation& simulation);

void assignAttackerDefender(Simulation& simulation);
void createAppliedEffectBattles(Simulation& simulation);
void removeConsideredEffect(Simulation& simulation);
void reapplyConsideredEffect(Simulation& simulation);
void postRunCleanup(Simulation& simulation);
}  // namespace analyze_effect
}  // namespace pokesim