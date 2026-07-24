#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/ItemProperty.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct LumBerry {
  static constexpr Item name(GameMechanics) { return dex::Item::LUM_BERRY; }

  static constexpr ItemProperty properties(GameMechanics) { return ItemProperty::BERRY; }
  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Lum Berry"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "lumberry"; }
  };

  static void onAfterSetStatus(Simulation& simulation);
  static void onUpdate(Simulation& simulation);
  static void onEat(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
