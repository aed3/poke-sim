#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <string_view>

#include "../Setup/DexDataTags.hpp"

namespace pokesim::dex {
template <GameMechanics>
struct KnockOff {
  static constexpr Move name = Move::KNOCK_OFF;
  static constexpr Type type = Type::DARK_TYPE;
  static constexpr MoveCategory category = MoveCategory::PHYSICAL;

  static constexpr types::BaseAccuracy accuracy = 100;
  static constexpr types::BasePower basePower = 65;
  static constexpr types::Pp basePp = 20;

  static constexpr internal::Tags<move::tags::AnySingleTarget, move::tags::Contact> moveTags{};

  struct Strings {
    static constexpr std::string_view name = "Knock Off";
    static constexpr std::string_view smogonId = "knockoff";
  };
};

namespace latest {
using KnockOff = dex::KnockOff<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex