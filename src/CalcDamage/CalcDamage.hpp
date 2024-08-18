#pragma once

#include <Components/CalcDamage/Aliases.hpp>
#include <Types/Damage.hpp>
#include <Types/Enums/DamageRollKind.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;
struct BasePower;
struct CurrentActionMoves;
struct Damage;
struct DamageRolls;
struct DamageRollModifier;
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

void checkForAndApplyStab(
  types::handle moveHandle, const Attacker& attacker, const TypeName& type, DamageRollModifier& modifier);
void checkForAndApplyTypeEffectiveness(
  types::handle moveHandle, const Attacker& attacker, const Defenders& defenders, const TypeName& type,
  DamageRollModifier& modifier);
void applyDamageRollsAndModifiers(Simulation& simulation, DamageRollKind damageRollKind, bool calculateUpToFoeHp);
void applyBurnModifier(types::registry& registry, const CurrentActionMoves& moves);
void setDamageToFirstRoll(const DamageRolls& damageRolls, Damage& damage);

void calculateAllDamageRolls(DamageRolls& damageRolls, const Damage& damage, const DamageRollModifier& modifier);

void applyAverageDamageRollAndModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifier& modifier);
void applyMinDamageRollAndModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifier& modifier);
void applyDamageModifier(DamageRolls& damageRolls, Damage damage, const DamageRollModifier& modifier);

void reduceDamageRollsToFoeHp(
  types::handle moveHandle, DamageRolls& damageRolls, const DamageRollModifier& modifier, const Defenders& defenders);
void setDamageToAtLeastOne(Damage& damage);
}  // namespace internal

void run(Simulation& simulation);
void clearRunVariables(Simulation& simulation);

void applyDamageRoll(Damage& damage, types::damageRoll damageRoll);
void applyAverageDamageRoll(Damage& damage);
void applyMinDamageRoll(Damage& damage);

void setDamageRollModifiers(Simulation& simulation);
template <typename SimulationTag>
void applyDamageRollsAndModifiers(Simulation& simulation);

void setIfMoveCrits(Simulation& simulation);
void getDamage(Simulation& simulation);
}  // namespace calc_damage
}  // namespace pokesim