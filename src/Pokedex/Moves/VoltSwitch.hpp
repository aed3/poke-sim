#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveProperty.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <string_view>

namespace pokesim::dex {
struct VoltSwitch {
  static constexpr Move name(GameMechanics) { return Move::VOLT_SWITCH; }
  static constexpr Type type(GameMechanics) { return Type::ELECTRIC; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::SPECIAL; }

  static constexpr types::baseAccuracy accuracy(GameMechanics) { return 100U; }
  static constexpr types::basePower basePower(GameMechanics) { return 70U; }
  static constexpr types::pp basePp(GameMechanics) { return 20U; }

  static constexpr MoveProperty properties(GameMechanics) { return MoveProperty::SELF_SWITCH; }
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ANY_SINGLE_TARGET; }

  struct Strings {
    static constexpr std::string_view name() { return "Volt Switch"; }
    static constexpr std::string_view smogonId() { return "voltswitch"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
