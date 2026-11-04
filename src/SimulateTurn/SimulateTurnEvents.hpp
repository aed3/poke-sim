#pragma once

#include <Types/State.hpp>

namespace pokesim {
class Simulation;

namespace internal::simulate_turn {
void runActiveUpdateEvents(Simulation& simulation);
void runOrderedEvent(Simulation& simulation, types::callback runEvent);
}  // namespace internal::simulate_turn
}  // namespace pokesim
