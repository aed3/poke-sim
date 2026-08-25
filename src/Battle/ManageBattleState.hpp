#pragma once

#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;
struct Battle;
struct Sides;
struct CurrentAction;
struct CurrentActionSource;
struct CurrentActionTarget;
struct RootBattle;

namespace internal {
void assignRootBattle(types::handle battleHandle);
void collectTurnOutcomeBattles(types::handle leafBattleHandle, RootBattle root);

void setCurrentActionSource(types::handle battleHandle, const Sides& sides, CurrentAction& action);
void setCurrentActionSwitchSource(types::handle battleHandle, const Sides& sides, CurrentAction& action);
void setCurrentActionSwitchTarget(types::handle battleHandle, const Sides& sides, CurrentAction& action);
void setCurrentActionMoveTarget(
  types::handle battleHandle, const Sides& sides, CurrentAction& action, const Simulation& simulation);
void setFailedActionMove(
  types::handle moveHandle, Battle battle, CurrentActionSource source, CurrentActionTarget target);
void clearMoveAction(Simulation& simulation);
void clearSwitchAction(Simulation& simulation);
}  // namespace internal
}  // namespace pokesim
