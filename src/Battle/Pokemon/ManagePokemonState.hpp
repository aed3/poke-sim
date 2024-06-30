#pragma once

#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;
struct CurrentActionSource;
struct CurrentActionMoveSlot;
struct Pp;
namespace stat {
struct Spe;
}  // namespace stat

void deductPp(Pp& pp);
void setLastMoveUsed(types::registry& registry, const CurrentActionSource& source, const CurrentActionMoveSlot& move);
void resetEffectiveSpeed(types::handle handle, stat::Spe spe);

void updateSpeed(Simulation& simulation);
}  // namespace pokesim