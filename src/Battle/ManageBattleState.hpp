#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
class Simulation;
class Pokedex;
struct Sides;
struct CurrentAction;
struct CurrentActionSource;

void setCurrentActionTarget(types::handle battleHandle, const Sides& sides, const CurrentAction& action);
void setCurrentActionSource(types::handle battleHandle, const Sides& sides, const CurrentAction& action);
void setCurrentActionMove(
  types::handle battleHandle, const Pokedex& pokedex, const CurrentAction& action, const CurrentActionSource& source);
void clearCurrentAction(Simulation& simulation);
}  // namespace pokesim