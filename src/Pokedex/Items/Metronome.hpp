#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/Volatile.hpp>
#include <Types/Event.hpp>
#include <array>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
namespace shared_name_effect {
struct MetronomeItem;
}

struct MetronomeItem {
  static constexpr Item name(GameMechanics = {}) { return dex::Item::METRONOME; }

  struct Strings {
    static constexpr std::string_view name() { return "Metronome"; }
    static constexpr std::string_view smogonId() { return "metronome"; }
  };

  using Effect = shared_name_effect::MetronomeItem;

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};

namespace shared_name_effect {
struct MetronomeItem {
  static constexpr Volatile name(GameMechanics = {}) { return Volatile::METRONOME; }
  using MainSource = pokesim::dex::MetronomeItem;

  static constexpr std::array<types::eventModifier, 6U> onModifyDamageModifiers(GameMechanics) {
    return {4096U, 4915U, 5734U, 6553U, 7372U, 8192U};
  }

  static void onStart(Simulation& simulation);
  static void onTryMove(Simulation& simulation);
  static void onModifyDamage(Simulation& simulation);
};
}  // namespace shared_name_effect
}  // namespace pokesim::dex
