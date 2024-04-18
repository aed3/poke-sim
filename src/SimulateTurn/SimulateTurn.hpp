#pragma once

namespace pokesim {
class Simulation;

namespace simulate_turn {
void run(Simulation& simulation);
void runActiveAction(Simulation& simulation);
void nextTurn(Simulation& simulation);

void runBeforeTurnAction(Simulation& simulation);
void runMoveAction(Simulation& simulation);
void runResidualAction(Simulation& simulation);
}  // namespace simulate_turn
}  // namespace pokesim