#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Stats.hpp>

namespace pokesim::calc_damage {
struct AttackingLevel {
  types::level val = MechanicConstants::PokemonLevel::MIN;
};

struct AttackingStat {
  types::stat val = MechanicConstants::PokemonEffectiveStat::MIN;
};

struct DefendingStat {
  types::stat val = MechanicConstants::PokemonEffectiveStat::MIN;
};

namespace tags {
struct P1Defending {};
struct P2Defending {};
}  // namespace tags
}  // namespace pokesim::calc_damage