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
}

namespace pokesim::dex {
struct Transform {
  static constexpr Move name(GameMechanics) { return Move::TRANSFORM; }
  static constexpr Type type(GameMechanics) { return Type::NORMAL; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::STATUS; }

  static constexpr types::pp basePp(GameMechanics) { return 10U; }

  static constexpr MoveProperty properties(GameMechanics) {
    return MoveProperty::NO_ENCORE | MoveProperty::NO_ASSIST | MoveProperty::NO_COPYCAT | MoveProperty::NO_MIMIC |
           MoveProperty::NO_INSTRUCT | MoveProperty::NO_MIRROR_MOVE | MoveProperty::NO_METRONOME;
  }
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::SELF; }

  struct Strings {
    static constexpr std::string_view name() { return "Transform"; }
    static constexpr std::string_view smogonId() { return "transform"; }
  };

  static void onHit(Simulation& Simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
