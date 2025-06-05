#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Event.hpp>
#include <string_view>

namespace pokesim {
struct EventModifier;
}  // namespace pokesim

namespace pokesim::dex {
namespace internal {
struct AssaultVestEvents {
  static void onModifySpd(EventModifier& eventModifier);
};
}  // namespace internal

template <GameMechanics>
struct AssaultVest : internal::AssaultVestEvents {
  static constexpr dex::Item name = dex::Item::ASSAULT_VEST;

  static constexpr types::effectMultiplier onModifySpdModifier = 1.5F;
  struct Strings {
    static constexpr std::string_view name = "Assault Vest";
    static constexpr std::string_view smogonId = "assaultvest";
  };
};

namespace latest {
using AssaultVest = dex::AssaultVest<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex