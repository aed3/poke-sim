#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Types/Stats.hpp>
#include <Utilities/Tags.hpp>
#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct QuiverDance {
  static constexpr Move name = Move::QUIVER_DANCE;
  static constexpr Type type = Type::BUG;
  static constexpr MoveCategory category = MoveCategory::STATUS;

  static constexpr types::pp basePp = 20U;

  struct sourcePrimaryEffect {
    static constexpr types::boost spaBoost = 1, spdBoost = 1, speBoost = 1;

    static constexpr Tags<> effectTags{};
  };

  static constexpr Tags<> moveTags{};
  static constexpr MoveTarget target = MoveTarget::SELF;

  struct Strings {
    static constexpr std::string_view name = "Quiver Dance";
    static constexpr std::string_view smogonId = "quiverdance";
  };
};

namespace latest {
using QuiverDance = dex::QuiverDance<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex