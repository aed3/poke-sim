#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Stats.hpp>

namespace pokesim::calc_damage {
struct RealEffectiveStat {
  types::stat val = MechanicConstants::PokemonEffectiveStat::MIN;
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
}  // namespace pokesim::calc_damage