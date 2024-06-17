#pragma once

#include <Components/RandomEventCheck.hpp>
#include <Types/Entity.hpp>
#include <Types/Move.hpp>

namespace pokesim {
class Simulation;
struct CurrentActionMove;
struct CurrentActionTargets;
struct Accuracy;
struct Battle;
struct HitCount;

namespace internal {
void assignMoveAccuracyToTargets(types::handle targetHandle, const CurrentActionMove& currentMove);
void removeAccuracyFromTargets(types::registry& registry, const CurrentActionTargets& targets);
void removeFailedAccuracyCheckTargets(types::registry& registry, const CurrentActionTargets& targets);

void assignHitCountToTargets(types::handle targetHandle, const CurrentActionMove& currentMove);
template <types::moveHits MoveHits>
void assignHitCountFromVariableHitChance(types::registry& registry, const CurrentActionTargets& targets);

void deductMoveHitCount(types::handle targetHandle, HitCount& hitCount);

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