#pragma once

namespace pokesim {
class Simulation;

namespace simulate_turn {
void run(Simulation& simulation);
void runActiveAction(Simulation& simulation);
void nextTurn(Simulation& simulation);
}
}  // namespace pokesim