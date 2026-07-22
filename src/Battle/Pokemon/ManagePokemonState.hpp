#pragma once

#include <Types/Damage.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Registry.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
class Simulation;
class Pokedex;
struct Battle;
struct CurrentAction;
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

namespace internal {
void setItem(pokesim::dex::Item item, const Pokedex& pokedex, types::registry& registry, types::entity entity);
void setAbility(pokesim::dex::Ability ability, const Pokedex& pokedex, types::registry& registry, types::entity entity);
void setStatus(pokesim::dex::Status status, types::registry& registry, types::entity entity);

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
void setLastMoveUsed(types::registry& registry, CurrentAction& source, const CurrentActionMoveSlot& move);

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
}  // namespace internal
}  // namespace pokesim
