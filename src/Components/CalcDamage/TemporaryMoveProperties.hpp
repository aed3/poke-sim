#pragma once

#include <Types/Constants.hpp>
#include <Types/Damage.hpp>
#include <Types/Enums/StabBoostKind.hpp>
#include <Types/Event.hpp>
#include <Types/FixedMemoryVector.hpp>
#include <Types/Move.hpp>
#include <Types/Stats.hpp>

namespace pokesim::internal::calc_damage {
struct RealEffectiveStat {
  types::stat val = Constants::PokemonEffectiveStat::DEFAULT;
};

struct DamageRollModifiers {
  StabBoostKind stab = StabBoostKind::NONE;
  types::typeEffectiveness typeEffectiveness = Constants::TypeEffectivenessShift::DEFAULT;
  bool burn = false;
  bool zOrMaxBrokenProtect = false;
  types::eventModifier damageEventModifier = Constants::FIXED_POINT_SCALE;

  bool operator==(const DamageRollModifiers& other) const {
    return stab == other.stab && typeEffectiveness == other.typeEffectiveness && burn == other.burn &&
           damageEventModifier == other.damageEventModifier && zOrMaxBrokenProtect == other.zOrMaxBrokenProtect;
  }
};

struct DamageFormulaVariables {
  types::level attackingLevel = Constants::PokemonLevel::DEFAULT;
  types::power power = Constants::MovePower::DEFAULT;
  types::stat attackingStat = Constants::PokemonEffectiveStat::DEFAULT;
  types::stat defendingStat = Constants::PokemonEffectiveStat::DEFAULT;

  constexpr bool operator==(const DamageFormulaVariables& other) const {
    return attackingLevel == other.attackingLevel && power == other.power && attackingStat == other.attackingStat &&
           defendingStat == other.defendingStat;
  }
};

struct TotalDamageDoneOverUses {
  types::moveUses uses = 1U;
  types::fixedMemoryVector<types::stat, Constants::DamageRollCount::MAX> hpRemaining{};

  constexpr bool operator==(const TotalDamageDoneOverUses& other) const {
    return uses == other.uses && hpRemaining == other.hpRemaining;
  }
};

namespace tags {
struct UsesAtk {};
struct UsesDef {};
struct UsesSpa {};
struct UsesSpd {};
struct UsesDefAsOffense {};  // For Body Press

struct IgnoresAttackingBoost {};
struct IgnoresDefendingBoost {};

struct NextSuccessiveUseMayChangeDamage {};
}  // namespace tags
}  // namespace pokesim::internal::calc_damage
