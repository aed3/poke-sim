#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveProperty.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Types/Random.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct CloseCombat {
  static constexpr Move name(GameMechanics = {}) { return Move::CLOSE_COMBAT; }
  static constexpr Type type(GameMechanics) { return Type::FIGHTING; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::PHYSICAL; }

  static constexpr types::baseAccuracy accuracy(GameMechanics) { return 100U; }
  static constexpr types::basePower basePower(GameMechanics) { return 120U; }
  static constexpr types::pp basePp(GameMechanics) { return 5U; }

  struct sourcePrimaryEffect {
    static constexpr types::boost defBoost(GameMechanics) { return -1; }
    static constexpr types::boost spdBoost(GameMechanics) { return -1; }
  };

  static constexpr MoveProperty properties(GameMechanics) { return MoveProperty::CONTACT; }
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ANY_SINGLE_TARGET; }

  struct Strings {
    static constexpr std::string_view name() { return "Close Combat"; }
    static constexpr std::string_view smogonId() { return "closecombat"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
