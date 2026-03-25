
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
template <GameMechanics>
struct Splash {
  static constexpr Move name = Move::SPLASH;
  static constexpr Type type = Type::NORMAL;
  static constexpr MoveCategory category = MoveCategory::STATUS;

  static constexpr types::pp basePp = 40U;

  static constexpr Tags<> moveTags{};
  static constexpr MoveTarget target = MoveTarget::SELF;

  struct Strings {
    static constexpr std::string_view name = "Splash";
    static constexpr std::string_view smogonId = "splash";
  };
};

namespace latest {
using Splash = dex::Splash<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex
