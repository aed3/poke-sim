#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
class Simulation;
struct ActiveAction;
struct ActiveSource;

void setActiveTarget(types::handle handle, ActiveAction activeAction, ActiveSource activeSource);
void setActiveMove(types::handle handle, ActiveAction activeAction);
void clearActive(Simulation& simulation);
}  // namespace pokesim