#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveProperty.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Types/Random.hpp>
#include <string_view>

namespace pokesim::dex {
struct ZapCannon {
  static constexpr Move name(GameMechanics = {}) { return Move::ZAP_CANNON; }
  static constexpr Type type(GameMechanics) { return Type::ELECTRIC; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::SPECIAL; }

  static constexpr types::baseAccuracy accuracy(GameMechanics) { return 50U; }
  static constexpr types::basePower basePower(GameMechanics) { return 120U; }
  static constexpr types::pp basePp(GameMechanics) { return 5U; }

  struct targetSecondaryEffect {
    static constexpr types::percentChance chance(GameMechanics) { return 100U; }

    static constexpr Status status(GameMechanics) { return Status::PAR; }
  };

  static constexpr MoveProperty properties(GameMechanics) { return MoveProperty::BULLET; }
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ANY_SINGLE_TARGET; }

  struct Strings {
    static constexpr std::string_view name() { return "Zap Cannon"; }
    static constexpr std::string_view smogonId() { return "zapcannon"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
