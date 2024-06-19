#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
class Simulation;
class Pokedex;
struct Sides;
struct CurrentAction;
struct CurrentActionSource;
struct CurrentActionTargets;

void setCurrentActionTarget(types::handle battleHandle, const Sides& sides, const CurrentAction& action);
void setCurrentActionSource(types::handle battleHandle, const Sides& sides, const CurrentAction& action);
void setCurrentActionMove(
  types::handle battleHandle, const CurrentActionSource& source, const CurrentActionTargets& targets,
  const CurrentAction& action, const Pokedex& pokedex);
void clearCurrentAction(Simulation& simulation);
}  // namespace pokesim