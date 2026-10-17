#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveProperty.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Event.hpp>
#include <Types/Move.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct Reversal {
  static constexpr Move name(GameMechanics = {}) { return Move::REVERSAL; }
  static constexpr Type type(GameMechanics) { return Type::FIGHTING; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::PHYSICAL; }

  static constexpr types::baseAccuracy accuracy(GameMechanics) { return 100U; }
  static constexpr types::pp basePp(GameMechanics) { return 15U; }

  static constexpr MoveProperty properties(GameMechanics) { return MoveProperty::CONTACT; }
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ANY_SINGLE_TARGET; }

  static constexpr types::basePower onSetBasePowerFromSourceHp(
    GameMechanics, types::stat sourceHp, types::stat sourceMaxHp) {
    // It's odd that the Bulbapedia definition and the Smogon definition of this move's calculation result in different
    // numbers, especially when the former uses such precise percentages.
    types::stat ratio = (48U * sourceHp) / sourceMaxHp;
    if (ratio < 2U) {
      return 200U;
    }
    if (ratio < 5U) {
      return 150U;
    }
    if (ratio < 10U) {
      return 100U;
    }
    if (ratio < 17U) {
      return 80U;
    }
    if (ratio < 33U) {
      return 40U;
    }
    return 20U;
  }

  struct Strings {
    static constexpr std::string_view name() { return "Reversal"; }
    static constexpr std::string_view smogonId() { return "reversal"; }
  };

  static void onSetBasePower(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
