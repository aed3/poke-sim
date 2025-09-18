#pragma once

namespace pokesim {
class Simulation;

void runAccuracyEvent(Simulation& simulation);
void runModifyAccuracyEvent(Simulation& simulation);
void runModifyCritBoostEvent(Simulation& simulation);
void runBasePowerEvent(Simulation& simulation);
void runDamagingHitEvent(Simulation& simulation);
void runModifySecondariesEvent(Simulation& simulation);

template <typename StatusType>
void runStatusImmunityEvent(Simulation& simulation);
void runAfterSetStatusEvent(Simulation& simulation);

void runChangeBoostEvent(Simulation& simulation);
void runTryBoostEvent(Simulation& simulation);
template <typename BoostType>
void runAfterEachBoostEvent(Simulation& simulation);
void runAfterBoostEvent(Simulation& simulation);
void runModifyMove(Simulation& simulation);
void runDisableMove(Simulation& simulation);

void runModifyAtk(Simulation& simulation);
void runModifyDef(Simulation& simulation);
void runModifySpa(Simulation& simulation);
void runModifySpd(Simulation& simulation);
void runModifySpe(Simulation& simulation);

void runStartSleep(Simulation& simulation);
void runStartFreeze(Simulation& simulation);
}  // namespace pokesim