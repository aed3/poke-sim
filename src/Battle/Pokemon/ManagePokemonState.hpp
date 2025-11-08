#pragma once

#include <Types/Damage.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Registry.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
class Simulation;
struct CurrentActionSource;
struct CurrentActionTarget;
struct CurrentActionMoveSlot;
struct Damage;
struct Pp;

namespace stat {
struct Atk;
struct Def;
struct Spa;
struct Spd;
struct Spe;
struct CurrentHp;
}  // namespace stat

void checkIfCanUseItem(Simulation& simulation);
void useItem(Simulation& simulation);
void tryUseItem(Simulation& simulation);
void checkIfCanRemoveItem(Simulation& simulation);
void removeItem(Simulation& simulation);
void tryRemoveItem(Simulation& simulation);

void checkIfCanSetStatus(Simulation& simulation);
void setStatus(Simulation& simulation);
void trySetStatus(Simulation& simulation);
void clearStatus(types::handle pokemonHandle);

void deductPp(Pp& pp);
void setLastMoveUsed(types::registry& registry, CurrentActionSource source, const CurrentActionMoveSlot& move);
void resetEffectiveAtk(types::handle handle, stat::Atk atk);
void resetEffectiveDef(types::handle handle, stat::Def def);
void resetEffectiveSpa(types::handle handle, stat::Spa spa);
void resetEffectiveSpd(types::handle handle, stat::Spd spd);
void resetEffectiveSpe(types::handle handle, stat::Spe spe);

void applyDamage(types::handle pokemonHandle, types::damage damage);
void applyStatBoost(types::stat& stat, types::boost boost);

void tryBoost(Simulation& simulation);

void updateAllStats(Simulation& simulation);
void updateAtk(Simulation& simulation);
void updateDef(Simulation& simulation);
void updateSpa(Simulation& simulation);
void updateSpd(Simulation& simulation);
void updateSpe(Simulation& simulation);
}  // namespace pokesim