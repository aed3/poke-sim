#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Types/Stats.hpp>
#include <string_view>

#include "../Setup/DexDataTags.hpp"

namespace pokesim::dex {
template <GameMechanics>
struct Moonblast {
  static constexpr Move name = Move::MOONBLAST;
  static constexpr Type type = Type::FAIRY_TYPE;
  static constexpr MoveCategory category = MoveCategory::SPECIAL;

  static constexpr types::BaseAccuracy accuracy = 100;
  static constexpr types::BasePower basePower = 95;
  static constexpr types::Pp basePp = 15;

  struct targetSecondaryEffect {
    static constexpr types::BaseEffectChance chance = 30;
    static constexpr types::Boost spaBoost = -1;

    static constexpr internal::Tags<> effectTags{};
  };

  static constexpr internal::Tags<move::tags::AnySingleTarget> moveTags{};

  struct Strings {
    static constexpr std::string_view name = "Moonblast";
    static constexpr std::string_view smogonId = "moonblast";
  };
};

namespace latest {
using Moonblast = dex::Moonblast<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex