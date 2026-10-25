#pragma once

namespace pokesim {
class Simulation;

namespace internal::simulate_turn {
void runMoveHitChecks(Simulation& simulation);
}
}  // namespace pokesim
