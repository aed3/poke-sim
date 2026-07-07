#pragma once

#include <Types/Constants.hpp>
#include <Types/Move.hpp>
#include <Types/Stats.hpp>

namespace pokesim::internal::calc_damage {
struct RealEffectiveStat {
  types::stat val = Constants::PokemonEffectiveStat::DEFAULT;
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

namespace tags {
struct UsesAtk {};
struct UsesDef {};
struct UsesSpa {};
struct UsesSpd {};
struct UsesDefAsOffense {};  // For Body Press

struct IgnoresAttackingBoost {};
struct IgnoresDefendingBoost {};
}  // namespace tags
}  // namespace pokesim::internal::calc_damage
