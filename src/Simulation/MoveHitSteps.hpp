#pragma once

#include <Components/RandomEventInputs.hpp>
#include <Types/Enums/BattleFormat.hpp>
#include <Types/Move.hpp>
#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;
struct CurrentActionSource;
struct CurrentActionMoves;
struct CurrentActionTargets;
struct Accuracy;
struct Battle;
struct HitCount;

namespace internal {
void deductMoveHitCount(types::handle moveHandle, HitCount& hitCount);

template <auto Function>
void runMoveHitCheck(Simulation& simulation);
void postMoveHitCheck(Simulation& simulation);
void updateCurrentActionTargets(types::registry& registry, CurrentActionTargets& targets);
void removeFailedHitTargets(
  types::handle moveTarget, const CurrentActionTargets& targets, const CurrentActionSource& source);
}  // namespace internal

void setMoveHitCount(Simulation& simulation);

void trySetStatusFromEffect(Simulation& simulation);
void applyDamage(Simulation& simulation);
void runSecondaryMoveEffects(Simulation& simulation);
void accuracyCheck(Simulation& simulation);
void moveHitLoop(Simulation& simulation);

void runMoveHitChecks(Simulation& simulation);
}  // namespace pokesim