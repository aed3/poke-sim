#pragma once

#include <Types/Entity.hpp>

namespace pokesim {
class Simulation;
struct Pp;
namespace stat {
struct Spe;
}  // namespace stat

void deductPp(Pp& pp);
void setLastMoveUsed(types::handle handle);
void resetEffectiveSpeed(types::handle handle, stat::Spe spe);

void updateSpeed(Simulation& simulation);
}  // namespace pokesim