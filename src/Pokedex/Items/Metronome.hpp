#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Volatile.hpp>
#include <Types/Event.hpp>
#include <Types/Move.hpp>
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

  static constexpr types::eventModifier onModifyDamageModifiers(GameMechanics, types::moveHits successiveHits) {
    switch (successiveHits) {
      case 0U: return 4096U;
      case 1U: return 4915U;
      case 2U: return 5734U;
      case 3U: return 6553U;
      case 4U: return 7372U;
      case 5U:
      default: return 8192U;
    }
  }

  static constexpr types::eventModifier onModifyDamageDenominator(GameMechanics) { return 4096U; }
  static constexpr types::moveHits onModifyDamageMaxSuccessiveHits(GameMechanics) { return 5U; }

  static void onMoveFail(Simulation& simulation);
  static void onTryMove(Simulation& simulation);
  static void onModifyDamage(Simulation& simulation);

  types::moveHits successiveHits{};
  Move lastMove = Move::NO_MOVE;

  constexpr bool operator==(const MetronomeItem& other) const {
    return successiveHits == other.successiveHits && lastMove == other.lastMove;
  }
};
}  // namespace shared_name_effect
}  // namespace pokesim::dex
