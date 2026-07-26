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

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct AllySwitch {
  static constexpr Move name(GameMechanics) { return Move::ALLY_SWITCH; }
  static constexpr Type type(GameMechanics) { return Type::PSYCHIC; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::STATUS; }

  static constexpr types::pp basePp(GameMechanics) { return 15U; }
  static constexpr types::priority priority(GameMechanics) { return 2; }

  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::SELF; }

  struct Strings {
    static constexpr std::string_view name() { return "Ally Switch"; }
    static constexpr std::string_view smogonId() { return "allyswitch"; }
  };

  static void onPrepareHit(Simulation& simulation);
  static void onHit(Simulation& simulation);
  struct Effect {
    static void onStart(Simulation& simulation);
    static void onRestart(Simulation& simulation);
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
