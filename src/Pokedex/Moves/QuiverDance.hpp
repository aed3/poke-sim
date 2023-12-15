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
struct QuiverDance {
  static constexpr Move name = Move::QUIVER_DANCE;
  static constexpr Type type = Type::BUG_TYPE;
  static constexpr MoveCategory category = MoveCategory::STATUS;

  static constexpr types::Pp basePp = 20;

  struct sourcePrimaryEffect {
    static constexpr types::Boost spaBoost = 1, spdBoost = 1, speBoost = 1;

    static constexpr internal::Tags<> effectTags{};
  };

  static constexpr internal::Tags<tags::move::Self> moveTags{};

  struct Strings {
    static constexpr std::string_view name = "Quiver Dance";
    static constexpr std::string_view smogonId = "quiverdance";
  };
};

namespace latest {
using QuiverDance = dex::QuiverDance<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex