#pragma once

namespace pokesim {
class Simulation;

void runAccuracyEvent(Simulation& simulation);
void runModifyAccuracyEvent(Simulation& simulation);
void runModifyCritBoostEvent(Simulation& simulation);
void runBasePowerEvent(Simulation& simulation);
void runModifyDamageEvent(Simulation& simulation);

// This is the damage modification part of the onDamage events to be run during calc damage so those numbers are
// reflected in the calculation. Affected events: Focus Sash, Disguise, Heat Proof, Ice Face, Sturdy, Endure, False
// Swipe, & Hold Back. Note that Focus Band is left out due to its randomness.
void runAfterModifyDamageEvent(Simulation& simulation);
void runDamageEvent(Simulation& simulation);
void runModifySecondariesEvent(Simulation& simulation);
void runDamagingHitEvent(Simulation& simulation);
void runAfterHitEvent(Simulation& simulation);
void runAfterMoveUsedEvent(Simulation& simulation);  // AfterMoveSecondarySelf

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

void runTryTakeItemEvent(Simulation& simulation);  // TakeItem
void runAfterUseItemEvent(Simulation& simulation);
void runEndItemEvent(Simulation& simulation);
}  // namespace pokesim