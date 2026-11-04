#pragma once

#include <Types/Constants.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/ItemProperty.hpp>
#include <Types/Enums/Volatile.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
namespace shared_name_effect {
struct MirrorHerb;
}

struct MirrorHerb {
  static constexpr Item name(GameMechanics = {}) { return dex::Item::MIRROR_HERB; }

  struct Strings {
    static constexpr std::string_view name() { return "Mirror Herb"; }
    static constexpr std::string_view smogonId() { return "mirrorherb"; }
  };

  static void onFoeAfterBoost(Simulation& simulation);

  using Effect = shared_name_effect::MirrorHerb;

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};

namespace shared_name_effect {
struct MirrorHerb {
  static constexpr Volatile name(GameMechanics = {}) { return Volatile::MIRROR_HERB; }
  using MainSource = pokesim::dex::MirrorHerb;

  static void onAnyAfterMega(Simulation& simulation);
  static void onAnyAfterMove(Simulation& simulation);
  static void onAnyAfterTerastallize(Simulation& simulation);
  static void onAnySwitchIn(Simulation& simulation);
  static void onResidual(Simulation& simulation);
  static void onUse(Simulation& simulation);
  static void onEnd(Simulation& simulation);

  types::boost atkBoost = Constants::PokemonStatBoost::DEFAULT;
  types::boost defBoost = Constants::PokemonStatBoost::DEFAULT;
  types::boost spaBoost = Constants::PokemonStatBoost::DEFAULT;
  types::boost spdBoost = Constants::PokemonStatBoost::DEFAULT;
  types::boost speBoost = Constants::PokemonStatBoost::DEFAULT;

  constexpr bool operator==(const MirrorHerb& other) const {
    return atkBoost == other.atkBoost && defBoost == other.defBoost && spaBoost == other.spaBoost &&
           spdBoost == other.spdBoost && speBoost == other.speBoost;
  }
};
}  // namespace shared_name_effect
}  // namespace pokesim::dex
