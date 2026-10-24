#pragma once

namespace pokesim {
class Simulation;

namespace internal::simulate_turn {
void runActiveUpdateEvents(Simulation& simulation);
}  // namespace internal::simulate_turn
}  // namespace pokesim
