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
  types::handle battleHandle, const Simulation& simulation, const CurrentActionSource& source,
  const CurrentAction& action);
void clearCurrentAction(Simulation& simulation);
}  // namespace pokesim