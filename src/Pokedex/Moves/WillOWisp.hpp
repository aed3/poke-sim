#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Utilities/Tags.hpp>
#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct WillOWisp {
  static constexpr Move name = Move::WILL_O_WISP;
  static constexpr Type type = Type::FIRE;
  static constexpr MoveCategory category = MoveCategory::STATUS;

  static constexpr types::baseAccuracy accuracy = 85U;
  static constexpr types::pp basePp = 15U;

  struct targetPrimaryEffect {
    static constexpr Tags<status::tags::Burn> effectTags{};
  };

  static constexpr Tags<> moveTags{};
  static constexpr MoveTarget target = MoveTarget::ANY_SINGLE_TARGET;

  struct Strings {
    static constexpr std::string_view name = "Will-O-Wisp";
    static constexpr std::string_view smogonId = "willowisp";
  };
};

namespace latest {
using WillOWisp = dex::WillOWisp<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex