#pragma once

#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;
class Pokedex;
struct Sides;
struct CurrentAction;
struct CurrentActionSource;
struct CurrentActionTargets;
struct RootBattle;

void assignRootBattle(types::handle battleHandle);
void collectTurnOutcomeBattles(types::handle leafBattleHandle, const RootBattle& root);

void setCurrentActionSource(types::handle battleHandle, const Sides& sides, CurrentAction action);
void setCurrentActionTarget(
  types::handle battleHandle, const Sides& sides, CurrentAction action, CurrentActionSource source);
void setCurrentActionMove(
  types::handle battleHandle, CurrentActionSource source, const CurrentActionTargets& targets, CurrentAction action,
  const Pokedex& pokedex);
void clearCurrentAction(Simulation& simulation);
}  // namespace pokesim