#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Stats.hpp>

namespace pokesim::stat {
struct Hp {
  types::stat val = MechanicConstants::PokemonHpStat::MIN;
};

struct Atk {
  types::stat val = MechanicConstants::PokemonStat::MIN;
};

struct Def {
  types::stat val = MechanicConstants::PokemonStat::MIN;
};

struct Spa {
  types::stat val = MechanicConstants::PokemonStat::MIN;
};

struct Spd {
  types::stat val = MechanicConstants::PokemonStat::MIN;
};

struct Spe {
  types::stat val = MechanicConstants::PokemonStat::MIN;
};

struct CurrentHp {
  types::stat val = MechanicConstants::PokemonHpStat::MIN;
};

struct EffectiveAtk {
  types::stat val = MechanicConstants::PokemonEffectiveStat::MIN;
};

struct EffectiveDef {
  types::stat val = MechanicConstants::PokemonEffectiveStat::MIN;
};

struct EffectiveSpa {
  types::stat val = MechanicConstants::PokemonEffectiveStat::MIN;
};

struct EffectiveSpd {
  types::stat val = MechanicConstants::PokemonEffectiveStat::MIN;
};

struct EffectiveSpe {
  types::stat val = MechanicConstants::PokemonEffectiveStat::MIN;
};
}  // namespace pokesim::stat
