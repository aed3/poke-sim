#pragma once

#include <Types/Damage.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
namespace events {
struct FocusSash {
  static void onAfterModifyDamage(Simulation& simulation);
  static void onDamage(Simulation& simulation);
};
}  // namespace events

template <GameMechanics>
struct FocusSash {
  static constexpr dex::Item name = dex::Item::FOCUS_SASH;

  static constexpr types::damage onAfterModifyDamageHpToKeep = 1U;
  struct Strings {
    static constexpr std::string_view name = "Focus Sash";
    static constexpr std::string_view smogonId = "focussash";
  };
};

namespace latest {
using FocusSash = dex::FocusSash<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex