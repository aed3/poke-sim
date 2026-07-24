#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Move.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}

namespace pokesim::dex {
struct Prankster {
  static constexpr Ability name(GameMechanics) { return dex::Ability::PRANKSTER; }

  static constexpr types::priority onModifyPriorityIncrease(GameMechanics) { return 1U; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Prankster"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "prankster"; }
  };

  static void onAfterEachBoost(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
