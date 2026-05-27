#pragma once

#include <Types/Stats.hpp>

namespace pokesim {
struct Evs {
  types::ev hp = Constants::PokemonEv::DEFAULT;
  types::ev atk = Constants::PokemonEv::DEFAULT;
  types::ev def = Constants::PokemonEv::DEFAULT;
  types::ev spa = Constants::PokemonEv::DEFAULT;
  types::ev spd = Constants::PokemonEv::DEFAULT;
  types::ev spe = Constants::PokemonEv::DEFAULT;

  bool operator==(const Evs& other) const {
    return hp == other.hp && atk == other.atk && def == other.def && spa == other.spa && spd == other.spd &&
           spe == other.spe;
  }
};

struct Ivs {
  types::iv hp = Constants::PokemonIv::DEFAULT;
  types::iv atk = Constants::PokemonIv::DEFAULT;
  types::iv def = Constants::PokemonIv::DEFAULT;
  types::iv spa = Constants::PokemonIv::DEFAULT;
  types::iv spd = Constants::PokemonIv::DEFAULT;
  types::iv spe = Constants::PokemonIv::DEFAULT;

  bool operator==(const Ivs& other) const {
    return hp == other.hp && atk == other.atk && def == other.def && spa == other.spa && spd == other.spd &&
           spe == other.spe;
  }
};
}  // namespace pokesim
