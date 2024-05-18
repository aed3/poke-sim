#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
class Simulation;
struct CurrentActionSource;
struct CurrentActionMove;
struct Pp;
namespace stat {
struct Spe;
}  // namespace stat

void deductPp(Pp& pp);
void setLastMoveUsed(types::registry& registry, const CurrentActionSource& source, const CurrentActionMove& move);
void resetEffectiveSpeed(types::handle handle, stat::Spe spe);

void updateSpeed(Simulation& simulation);
}  // namespace pokesim