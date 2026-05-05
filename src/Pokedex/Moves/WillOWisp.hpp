#pragma once

#include <Components/Tags/MovePropertyTags.hpp>
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
struct WillOWisp {
  static constexpr Move name(GameMechanics) { return Move::WILL_O_WISP; }
  static constexpr Type type(GameMechanics) { return Type::FIRE; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::STATUS; }

  static constexpr types::baseAccuracy accuracy(GameMechanics) { return 85U; }
  static constexpr types::pp basePp(GameMechanics) { return 15U; }

  struct targetPrimaryEffect {
    static constexpr Tags<status::tags::Burn> effectTags{};
  };

  static constexpr Tags<> moveTags{};
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ANY_SINGLE_TARGET; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Will-O-Wisp"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "willowisp"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
