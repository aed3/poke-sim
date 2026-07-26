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

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct Reflect {
  static constexpr Move name(GameMechanics) { return Move::REFLECT; }
  static constexpr Type type(GameMechanics) { return Type::PSYCHIC; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::STATUS; }

  static constexpr types::pp basePp(GameMechanics) { return 20U; }

  static constexpr MoveProperty properties(GameMechanics) { return MoveProperty::CAN_SNATCH; }
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ALLY_SIDE; }

  struct Strings {
    static constexpr std::string_view name() { return "Reflect"; }
    static constexpr std::string_view smogonId() { return "reflect"; }
  };

  struct Effect {
    static void onDuration(Simulation& simulation);
    static void onAnyModifyDamage(Simulation& simulation);
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
