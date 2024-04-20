#pragma once

namespace pokesim {
class Simulation;

void setMoveHitCount(Simulation& simulation);

void trySetStatusFromEffect(Simulation& simulation);
void applyDamage(Simulation& simulation);
void runSecondaryMoveEffects(Simulation& simulation);

void accuracyRandomChance(Simulation& simulation);
void accuracyCheckStep(Simulation& simulation);
void moveHitStep(Simulation& simulation);

void runMoveHitSteps(Simulation& simulation);
}  // namespace pokesim