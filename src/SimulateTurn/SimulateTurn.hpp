#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
class Simulation;
struct Battle;
struct CurrentActionTargets;

namespace simulate_turn {
void run(Simulation& simulation);
void runCurrentAction(Simulation& simulation);
void nextTurn(Simulation& simulation);

void runBeforeTurnAction(Simulation& simulation);
void runMoveAction(Simulation& simulation);
void runResidualAction(Simulation& simulation);

void addTargetAllyToTargets(types::registry& registry, const Battle& battle);
void addUserAllyToTargets(types::registry& registry, const Battle& battle);
void resolveMoveTargets(CurrentActionTargets&);
void getMoveTargets(Simulation& simulation);

void useMove(Simulation& simulation);
}  // namespace simulate_turn
}  // namespace pokesim