#pragma once

#include <Types/Constants.hpp>
#include <Types/Stats.hpp>

namespace pokesim::stat {
struct Hp {
  types::stat val = Constants::PokemonHpStat::DEFAULT;
};

struct Atk {
  types::stat val = Constants::PokemonStat::DEFAULT;
};

struct Def {
  types::stat val = Constants::PokemonStat::DEFAULT;
};

struct Spa {
  types::stat val = Constants::PokemonStat::DEFAULT;
};

struct Spd {
  types::stat val = Constants::PokemonStat::DEFAULT;
};

struct Spe {
  types::stat val = Constants::PokemonStat::DEFAULT;
};

struct CurrentHp {
  types::stat val = Constants::PokemonCurrentHpStat::DEFAULT;
};

struct EffectiveAtk {
  types::stat val = Constants::PokemonEffectiveStat::DEFAULT;
};

struct EffectiveDef {
  types::stat val = Constants::PokemonEffectiveStat::DEFAULT;
};

struct EffectiveSpa {
  types::stat val = Constants::PokemonEffectiveStat::DEFAULT;
};

struct EffectiveSpd {
  types::stat val = Constants::PokemonEffectiveStat::DEFAULT;
};

struct EffectiveSpe {
  types::stat val = Constants::PokemonEffectiveStat::DEFAULT;
};
}  // namespace pokesim::stat
