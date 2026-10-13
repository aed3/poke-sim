#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveProperty.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct NobleRoar {
  static constexpr Move name(GameMechanics = {}) { return Move::NOBLE_ROAR; }
  static constexpr Type type(GameMechanics) { return Type::NORMAL; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::STATUS; }

  static constexpr types::baseAccuracy accuracy(GameMechanics) { return 100U; }
  static constexpr types::pp basePp(GameMechanics) { return 30U; }

  struct targetPrimaryEffect {
    static constexpr types::boost atkBoost(GameMechanics) { return -1; }
    static constexpr types::boost spaBoost(GameMechanics) { return -1; }
  };

  static constexpr MoveProperty properties(GameMechanics) {
    return MoveProperty::SOUND | MoveProperty::BYPASS_SUBSTITUTE;
  }
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ANY_SINGLE_TARGET; }

  struct Strings {
    static constexpr std::string_view name() { return "Noble Roar"; }
    static constexpr std::string_view smogonId() { return "nobleroar"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
