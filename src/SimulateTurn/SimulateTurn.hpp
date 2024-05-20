#pragma once

namespace pokesim {
class Simulation;

namespace simulate_turn {
void run(Simulation& simulation);
void runCurrentAction(Simulation& simulation);
void nextTurn(Simulation& simulation);

void runBeforeTurnAction(Simulation& simulation);
void runMoveAction(Simulation& simulation);
void runResidualAction(Simulation& simulation);

void getMoveTargets(Simulation& simulation);
void useMove(Simulation& simulation);
}  // namespace simulate_turn
}  // namespace pokesim