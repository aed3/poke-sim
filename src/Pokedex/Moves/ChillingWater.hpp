#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Types/Random.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct ChillingWater {
  static constexpr Move name(GameMechanics = {}) { return Move::CHILLING_WATER; }
  static constexpr Type type(GameMechanics) { return Type::WATER; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::SPECIAL; }

  static constexpr types::baseAccuracy accuracy(GameMechanics) { return 100U; }
  static constexpr types::basePower basePower(GameMechanics) { return 50U; }
  static constexpr types::pp basePp(GameMechanics) { return 20U; }

  struct targetSecondaryEffect {
    static constexpr types::percentChance chance(GameMechanics) { return 100U; }
    static constexpr types::boost atkBoost(GameMechanics) { return -1; }
  };

  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ANY_SINGLE_TARGET; }

  struct Strings {
    static constexpr std::string_view name() { return "Chilling Water"; }
    static constexpr std::string_view smogonId() { return "chillingwater"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
