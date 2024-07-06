#pragma once

#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;
struct CurrentActionSource;
struct CurrentActionMoveSlot;
struct Pp;
namespace stat {
struct Atk;
struct Def;
struct Spa;
struct Spd;
struct Spe;
}  // namespace stat

void deductPp(Pp& pp);
void setLastMoveUsed(types::registry& registry, const CurrentActionSource& source, const CurrentActionMoveSlot& move);
void resetEffectiveAtk(types::handle handle, stat::Atk atk);
void resetEffectiveDef(types::handle handle, stat::Def def);
void resetEffectiveSpa(types::handle handle, stat::Spa spa);
void resetEffectiveSpd(types::handle handle, stat::Spd spd);
void resetEffectiveSpe(types::handle handle, stat::Spe spe);

void updateAllStats(Simulation& simulation);
void updateAtk(Simulation& simulation);
void updateDef(Simulation& simulation);
void updateSpa(Simulation& simulation);
void updateSpd(Simulation& simulation);
void updateSpe(Simulation& simulation);
}  // namespace pokesim