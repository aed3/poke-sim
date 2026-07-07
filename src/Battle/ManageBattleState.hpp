#pragma once

#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;
class Pokedex;
struct Battle;
struct Sides;
struct CurrentAction;
struct CurrentActionSource;
struct CurrentActionTarget;
struct CurrentActionTargets;
struct RootBattle;

namespace internal {
void assignRootBattle(types::handle battleHandle);
void collectTurnOutcomeBattles(types::handle leafBattleHandle, const RootBattle& root);

void setCurrentActionSource(types::handle battleHandle, const Sides& sides, CurrentAction action);
void setCurrentActionTarget(
  types::handle battleHandle, const Sides& sides, CurrentAction action, const Simulation& simulation);
void setFailedActionMove(
  types::handle moveHandle, Battle battle, CurrentActionSource source, CurrentActionTarget target);
void clearCurrentAction(Simulation& simulation);
}  // namespace internal
}  // namespace pokesim
