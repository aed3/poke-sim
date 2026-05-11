#pragma once

#include <Types/Stats.hpp>

namespace pokesim {
struct Evs {
  types::ev hp = MechanicConstants::PokemonEv::DEFAULT;
  types::ev atk = MechanicConstants::PokemonEv::DEFAULT;
  types::ev def = MechanicConstants::PokemonEv::DEFAULT;
  types::ev spa = MechanicConstants::PokemonEv::DEFAULT;
  types::ev spd = MechanicConstants::PokemonEv::DEFAULT;
  types::ev spe = MechanicConstants::PokemonEv::DEFAULT;

  bool operator==(const Evs& other) const {
    return hp == other.hp && atk == other.atk && def == other.def && spa == other.spa && spd == other.spd &&
           spe == other.spe;
  }
};

struct Ivs {
  types::iv hp = MechanicConstants::PokemonIv::DEFAULT;
  types::iv atk = MechanicConstants::PokemonIv::DEFAULT;
  types::iv def = MechanicConstants::PokemonIv::DEFAULT;
  types::iv spa = MechanicConstants::PokemonIv::DEFAULT;
  types::iv spd = MechanicConstants::PokemonIv::DEFAULT;
  types::iv spe = MechanicConstants::PokemonIv::DEFAULT;

  bool operator==(const Ivs& other) const {
    return hp == other.hp && atk == other.atk && def == other.def && spa == other.spa && spd == other.spd &&
           spe == other.spe;
  }
};
}  // namespace pokesim
