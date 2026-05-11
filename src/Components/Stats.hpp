#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Stats.hpp>

namespace pokesim::stat {
struct Hp {
  types::stat val = MechanicConstants::PokemonHpStat::DEFAULT;
};

struct Atk {
  types::stat val = MechanicConstants::PokemonStat::DEFAULT;
};

struct Def {
  types::stat val = MechanicConstants::PokemonStat::DEFAULT;
};

struct Spa {
  types::stat val = MechanicConstants::PokemonStat::DEFAULT;
};

struct Spd {
  types::stat val = MechanicConstants::PokemonStat::DEFAULT;
};

struct Spe {
  types::stat val = MechanicConstants::PokemonStat::DEFAULT;
};

struct CurrentHp {
  types::stat val = MechanicConstants::PokemonCurrentHpStat::DEFAULT;
};

struct EffectiveAtk {
  types::stat val = MechanicConstants::PokemonEffectiveStat::DEFAULT;
};

struct EffectiveDef {
  types::stat val = MechanicConstants::PokemonEffectiveStat::DEFAULT;
};

struct EffectiveSpa {
  types::stat val = MechanicConstants::PokemonEffectiveStat::DEFAULT;
};

struct EffectiveSpd {
  types::stat val = MechanicConstants::PokemonEffectiveStat::DEFAULT;
};

struct EffectiveSpe {
  types::stat val = MechanicConstants::PokemonEffectiveStat::DEFAULT;
};
}  // namespace pokesim::stat
