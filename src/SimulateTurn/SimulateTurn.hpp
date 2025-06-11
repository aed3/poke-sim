#pragma once

#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;
class Pokedex;
struct Battle;
struct CurrentActionTargets;
struct CurrentActionSource;

namespace simulate_turn {
void run(Simulation& simulation);
}  // namespace simulate_turn
}  // namespace pokesim