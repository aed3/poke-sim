#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
class Simulation;
struct CurrentActionTargets;
struct Accuracy;
struct Battle;

namespace internal {
void assignMoveAccuracyToTargets(types::handle targetHandle, const Battle& battle);
void removeAccuracyFromTargets(types::registry& registry, const CurrentActionTargets& targets);
void assignAccuracyToRandomEvent(types::registry& registry, const Simulation& simulation, const Battle& battle, const Accuracy& accuracy);

void assignHitTags(types::registry& registry, const CurrentActionTargets& targets);
void removeFailedHitTargets(types::registry& registry, CurrentActionTargets& targets);
}  // namespace internal

void setMoveHitCount(Simulation& simulation);

void trySetStatusFromEffect(Simulation& simulation);
void applyDamage(Simulation& simulation);
void runSecondaryMoveEffects(Simulation& simulation);
void accuracyCheck(Simulation& simulation);
void moveHitLoop(Simulation& simulation);

void runMoveHitChecks(Simulation& simulation);
}  // namespace pokesim