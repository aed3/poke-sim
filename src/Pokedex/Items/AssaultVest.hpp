#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Event.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
namespace events {
struct AssaultVest {
  static void onModifySpd(Simulation& simulation);
};
}  // namespace events

template <GameMechanics>
struct AssaultVest : events::AssaultVest {
  static constexpr dex::Item name = dex::Item::ASSAULT_VEST;

  static constexpr types::effectMultiplier onModifySpdModifier = 1.5F;
  struct Strings {
    static constexpr std::string_view name = "Assault Vest";
    static constexpr std::string_view smogonId = "assaultvest";
  };
};

namespace latest {
using AssaultVest = dex::AssaultVest<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex