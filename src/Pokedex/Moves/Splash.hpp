
#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Utilities/Tags.hpp>
#include <string_view>

namespace pokesim::dex {
struct Splash {
  static constexpr Move name(GameMechanics) { return Move::SPLASH; }
  static constexpr Type type(GameMechanics) { return Type::NORMAL; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::STATUS; }

  static constexpr types::pp basePp(GameMechanics) { return 40U; }

  static constexpr Tags<> moveTags{};
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::SELF; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Splash"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "splash"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
