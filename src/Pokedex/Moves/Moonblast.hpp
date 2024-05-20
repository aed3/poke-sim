#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveTarget.hpp>
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

  static constexpr types::baseAccuracy accuracy = 100;
  static constexpr types::basePower basePower = 95;
  static constexpr types::pp basePp = 15;

  struct targetSecondaryEffect {
    static constexpr types::baseEffectChance chance = 30;
    static constexpr types::boost spaBoost = -1;
  };

  static constexpr MoveTarget target = MoveTarget::ANY_SINGLE_TARGET;

  struct Strings {
    static constexpr std::string_view name = "Moonblast";
    static constexpr std::string_view smogonId = "moonblast";
  };
};

namespace latest {
using Moonblast = dex::Moonblast<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex