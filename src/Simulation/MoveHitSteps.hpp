#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
class Simulation;

void setMoveHitCount(Simulation& simulation);

void trySetStatusFromEffect(Simulation& simulation);
void applyDamage(Simulation& simulation);
void runSecondaryMoveEffects(Simulation& simulation);
void accuracyCheck(Simulation& simulation);
void moveHitLoop(Simulation& simulation);

void runMoveHitSteps(Simulation& simulation);
}  // namespace pokesim