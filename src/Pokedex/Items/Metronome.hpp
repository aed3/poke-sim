#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Event.hpp>
#include <array>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct MetronomeItem {
  static constexpr Item name(GameMechanics) { return dex::Item::METRONOME; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Metronome"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "metronome"; }
  };

  static void onStart(Simulation& simulation);
  struct Effect {
    static constexpr std::array<types::eventModifier, 6U> onModifyDamageModifiers(GameMechanics) {
      return {4096U, 4915U, 5734U, 6553U, 7372U, 8192U};
    }

    static void onStart(Simulation& simulation);
    static void onTryMove(Simulation& simulation);
    static void onModifyDamage(Simulation& simulation);
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
