#pragma once

#include <Types/Stats.hpp>

namespace pokesim {
struct Evs {
  types::ev hp = MechanicConstants::PokemonEv::MIN;
  types::ev atk = MechanicConstants::PokemonEv::MIN;
  types::ev def = MechanicConstants::PokemonEv::MIN;
  types::ev spa = MechanicConstants::PokemonEv::MIN;
  types::ev spd = MechanicConstants::PokemonEv::MIN;
  types::ev spe = MechanicConstants::PokemonEv::MIN;

  bool operator==(const Evs& other) const {
    return hp == other.hp && atk == other.atk && def == other.def && spa == other.spa && spd == other.spd &&
           spe == other.spe;
  }
};

struct Ivs {
  types::iv hp = MechanicConstants::PokemonIv::MIN;
  types::iv atk = MechanicConstants::PokemonIv::MIN;
  types::iv def = MechanicConstants::PokemonIv::MIN;
  types::iv spa = MechanicConstants::PokemonIv::MIN;
  types::iv spd = MechanicConstants::PokemonIv::MIN;
  types::iv spe = MechanicConstants::PokemonIv::MIN;

  bool operator==(const Ivs& other) const {
    return hp == other.hp && atk == other.atk && def == other.def && spa == other.spa && spd == other.spd &&
           spe == other.spe;
  }
};
}  // namespace pokesim
