#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveProperty.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Event.hpp>
#include <Types/Move.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct Reversal {
  static constexpr Move name(GameMechanics) { return Move::REVERSAL; }
  static constexpr Type type(GameMechanics) { return Type::FIGHTING; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::PHYSICAL; }

  static constexpr types::baseAccuracy accuracy(GameMechanics) { return 100U; }
  static constexpr types::pp basePp(GameMechanics) { return 15U; }

  static constexpr MoveProperty properties(GameMechanics) { return MoveProperty::CONTACT; }
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ANY_SINGLE_TARGET; }

  struct Strings {
    static constexpr std::string_view name() { return "Reversal"; }
    static constexpr std::string_view smogonId() { return "reversal"; }
  };

  static void onBasePower(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
