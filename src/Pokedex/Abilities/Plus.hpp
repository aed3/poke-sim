#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Event.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct Plus {
  static constexpr dex::Ability name(GameMechanics) { return dex::Ability::PLUS; }

  static constexpr types::effectMultiplier onModifySpaModifier(GameMechanics) { return 1.5F; }
  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Plus"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "plus"; }
  };

  static void onModifySpA(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
