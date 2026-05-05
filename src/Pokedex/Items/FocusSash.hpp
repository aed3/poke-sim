#pragma once

#include <Types/Damage.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Utilities/Tags.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct FocusSash {
  static constexpr dex::Item name(GameMechanics) { return dex::Item::FOCUS_SASH; }

  static constexpr types::damage onAfterModifyDamageHpToKeep(GameMechanics) { return 1U; }

  static constexpr Tags<> itemTags{};
  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Focus Sash"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "focussash"; }
  };

  static void onAfterModifyDamage(Simulation& simulation);
  static void onDamage(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
