#pragma once

namespace pokesim {
class Simulation;

void setMoveHitCount(Simulation& simulation);

void accuracyCheckStep(Simulation& simulation);
void moveHitStep(Simulation& simulation);

void runMoveHitSteps(Simulation& simulation);
}  // namespace pokesim