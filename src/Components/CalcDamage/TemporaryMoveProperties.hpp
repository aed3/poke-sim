#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Move.hpp>
#include <Types/Stats.hpp>

namespace pokesim::calc_damage {
struct RealEffectiveStat {
  types::stat val = MechanicConstants::PokemonEffectiveStat::MIN;
};

struct Power {
  types::power val = MechanicConstants::MovePower::MIN;
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