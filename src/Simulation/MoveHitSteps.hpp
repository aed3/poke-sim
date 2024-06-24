#pragma once

#include <Components/RandomEventCheck.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/BattleFormat.hpp>
#include <Types/Move.hpp>

namespace pokesim {
class Simulation;
struct CurrentActionSource;
struct CurrentActionMove;
struct CurrentActionTargets;
struct Accuracy;
struct Battle;
struct HitCount;

namespace internal {
void assignMoveAccuracyToTargets(types::handle targetHandle, const CurrentActionMove& currentMove);

template <BattleFormat Format>
void assignHitCountToTargets(types::handle targetHandle, const CurrentActionMove& currentMove);

void deductMoveHitCount(types::handle targetHandle, HitCount& hitCount);

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