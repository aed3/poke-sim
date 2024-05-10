#pragma once

namespace pokesim {
class Simulation;

void runAccuracyEvent(Simulation& simulation);
void runModifyAccuracyEvent(Simulation& simulation);
void runModifyCritRatioEvent(Simulation& simulation);
void runBasePowerEvent(Simulation& simulation);
void runDamagingHitEvent(Simulation& simulation);

void runModifySpe(Simulation& simulation);
}  // namespace pokesim