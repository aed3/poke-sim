#pragma once

#include <Components/CalcDamage/Aliases.hpp>
#include <Types/Damage.hpp>
#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;
struct BasePower;
struct Damage;
struct RandomEventIndex;
struct TypeName;

namespace calc_damage {
struct CritBoost;
struct AttackingLevel;
struct AttackingStat;
struct DefendingStat;

namespace internal {
void assignCritChanceDivisor(types::handle moveHandle, const CritBoost& critBoost);
void setSourceLevel(types::handle moveHandle, const Attacker& attacker);
template <typename Category>
void setUsedAttackStat(types::handle moveHandle, const Attacker& attacker);
template <typename Category>
void setUsedDefenseStat(types::handle moveHandle, const Defenders& defenders);

void calculateBaseDamage(
  types::handle moveHandle, const BasePower& basePower, const AttackingLevel& level, const AttackingStat& attack,
  const DefendingStat& defense);
void applyCritDamageIncrease(Damage& damage);
void setDefendingSide(types::handle moveHandle, const Defenders& defenders);
void checkForAndApplyStab(types::handle moveHandle, const Attacker& attacker, const TypeName& type, Damage& damage);
}  // namespace internal

void run(Simulation& simulation);
void clearRunVariables(Simulation& simulation);

void applyDamageRoll(Damage& damage, types::damageRoll damageRoll);
void applyAverageDamageRoll(Damage& damage);
void applyMinDamageRoll(Damage& damage);

void modifyDamageWithTypes(Simulation& simulation);
void getDamageRole(Simulation& simulation);

void setIfMoveCrits(Simulation& simulation);
void getDamage(Simulation& simulation);
}  // namespace calc_damage
}  // namespace pokesim