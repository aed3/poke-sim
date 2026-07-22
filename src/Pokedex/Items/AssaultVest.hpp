#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Event.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct AssaultVest {
  static constexpr dex::Item name(GameMechanics) { return dex::Item::ASSAULT_VEST; }

  static constexpr types::effectMultiplier onModifySpdModifier(GameMechanics) { return 1.5F; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Assault Vest"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "assaultvest"; }
  };

  static void onModifySpd(Simulation& simulation);
  static void onEnd(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
