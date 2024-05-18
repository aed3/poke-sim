#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
class Simulation;
struct Sides;
struct CurrentAction;
struct CurrentActionSource;

void setCurrentActionTarget(types::handle handle, const Sides& sides, const CurrentAction& action);
void setCurrentActionSource(types::handle handle, const Sides& sides, const CurrentAction& action);
void setCurrentActionMove(types::handle handle, const CurrentAction& action, const CurrentActionSource& source);
void clearCurrentAction(Simulation& simulation);
}  // namespace pokesim