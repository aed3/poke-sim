#pragma once

#include <Types/Constants.hpp>
#include <Types/Stats.hpp>

namespace pokesim::calc_damage {
struct AttackingLevel {
  types::level val = Constants::PokemonLevel::DEFAULT;
};

struct AttackingStat {
  types::stat val = Constants::PokemonEffectiveStat::DEFAULT;
};

struct DefendingStat {
  types::stat val = Constants::PokemonEffectiveStat::DEFAULT;
};
}  // namespace pokesim::calc_damage
