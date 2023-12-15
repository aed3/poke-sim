#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <string_view>

#include "../Setup/DexDataTags.hpp"

namespace pokesim::dex {
template <GameMechanics>
struct WillOWisp {
  static constexpr Move name = Move::WILL_O_WISP;
  static constexpr Type type = Type::FIRE_TYPE;
  static constexpr MoveCategory category = MoveCategory::STATUS;

  static constexpr types::BaseAccuracy accuracy = 85;
  static constexpr types::Pp basePp = 15;

  struct targetPrimaryEffect {
    static constexpr internal::Tags<tags::status::Burn> effectTags{};
  };

  static constexpr internal::Tags<tags::move::AnySingleTarget> moveTags{};

  struct Strings {
    static constexpr std::string_view name = "Will-O-Wisp";
    static constexpr std::string_view smogonId = "willowisp";
  };
};

namespace latest {
using WillOWisp = dex::WillOWisp<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex