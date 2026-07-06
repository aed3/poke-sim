#pragma once

#include <Types/Damage.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Registry.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
class Simulation;
struct Battle;
struct CurrentActionSource;
struct CurrentActionTarget;
struct CurrentActionMoveSlot;
struct Damage;
struct LastUsedMove;
struct MoveSlots;

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

void clearVolatiles(types::handle pokemonHandle);

void deductPp(MoveSlots& moveSlots, LastUsedMove lastUsedMove);
void setLastMoveUsed(types::registry& registry, CurrentActionSource source, const CurrentActionMoveSlot& move);

void faint(types::handle pokemonHandle, Battle battle);
void applyDamage(types::handle pokemonHandle, types::damage damage);
void applyStatBoost(types::stat& stat, types::boost boost);

void tryBoost(Simulation& simulation);

void updateAllStats(Simulation& simulation);
void updateAtk(Simulation& simulation, bool ignoreBoosts);
void updateDef(Simulation& simulation, bool ignoreBoosts);
void updateSpa(Simulation& simulation, bool ignoreBoosts);
void updateSpd(Simulation& simulation, bool ignoreBoosts);
void updateSpe(Simulation& simulation, bool ignoreBoosts);
}  // namespace pokesim
