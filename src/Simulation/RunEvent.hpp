#pragma once

namespace pokesim {
class Simulation;

void runAccuracyEvent(Simulation& simulation);
void runModifyAccuracyEvent(Simulation& simulation);
void runModifyCritBoostEvent(Simulation& simulation);
void runBasePowerEvent(Simulation& simulation);
void runDamagingHitEvent(Simulation& simulation);
void runModifyMove(Simulation& simulation);

void runModifyAtk(Simulation& simulation);
void runModifyDef(Simulation& simulation);
void runModifySpa(Simulation& simulation);
void runModifySpd(Simulation& simulation);
void runModifySpe(Simulation& simulation);
}  // namespace pokesim