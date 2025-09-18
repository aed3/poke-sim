#pragma once

#include <Components/RandomEventInputs.hpp>
#include <Types/Enums/BattleFormat.hpp>
#include <Types/Move.hpp>
#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;

void setMoveHitCount(Simulation& simulation);

void applyDamage(Simulation& simulation);
void runPrimaryMoveEffects(Simulation& simulation);
void runSecondaryMoveEffects(Simulation& simulation);
void accuracyCheck(Simulation& simulation);
void moveHitLoop(Simulation& simulation);

void runMoveHitChecks(Simulation& simulation);
}  // namespace pokesim