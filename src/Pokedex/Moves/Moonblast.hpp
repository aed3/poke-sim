#pragma once

#include <Components/Tags/MovePropertyTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Types/Random.hpp>
#include <Types/Stats.hpp>
#include <Utilities/Tags.hpp>
#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct Moonblast {
  static constexpr Move name = Move::MOONBLAST;
  static constexpr Type type = Type::FAIRY;
  static constexpr MoveCategory category = MoveCategory::SPECIAL;

  static constexpr types::baseAccuracy accuracy = 100U;
  static constexpr types::basePower basePower = 95U;
  static constexpr types::pp basePp = 15U;

  struct targetSecondaryEffect {
    static constexpr types::percentChance chance = 30U;
    static constexpr types::boost spaBoost = -1;

    static constexpr Tags<> effectTags{};
  };

  static constexpr Tags<> moveTags{};
  static constexpr MoveTarget target = MoveTarget::ANY_SINGLE_TARGET;

  struct Strings {
    static constexpr std::string_view name = "Moonblast";
    static constexpr std::string_view smogonId = "moonblast";
  };
};

namespace latest {
using Moonblast = dex::Moonblast<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex