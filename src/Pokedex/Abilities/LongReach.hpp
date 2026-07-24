#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}

namespace pokesim::dex {
struct LongReach {
  static constexpr Ability name(GameMechanics) { return dex::Ability::LONG_REACH; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Long Reach"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "longreach"; }
  };

  static void onModifyMove(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
