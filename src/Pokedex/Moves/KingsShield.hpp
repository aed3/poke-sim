#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveProperty.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/Volatile.hpp>
#include <Types/Move.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
namespace shared_name_effect {
struct KingsShield;
}

struct KingsShield {
  static constexpr Move name(GameMechanics = {}) { return Move::KINGS_SHIELD; }
  static constexpr Type type(GameMechanics) { return Type::STEEL; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::STATUS; }

  static constexpr types::pp basePp(GameMechanics) { return 10U; }
  static constexpr types::priority priority(GameMechanics) { return 4; }

  struct targetPrimaryEffect {
    static constexpr Volatile volatileCondition(GameMechanics) { return Volatile::KINGS_SHIELD; }

    static void onHit(Simulation& simulation);
  };

  static constexpr MoveProperty properties(GameMechanics) {
    return MoveProperty::NO_ASSIST | MoveProperty::NO_COPYCAT | MoveProperty::NO_INSTRUCT |
           MoveProperty::NO_MIRROR_MOVE | MoveProperty::NO_METRONOME | MoveProperty::STALL;
  }
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::SELF; }

  struct Strings {
    static constexpr std::string_view name() { return "King's Shield"; }
    static constexpr std::string_view smogonId() { return "kingsshield"; }
  };

  using Effect = shared_name_effect::KingsShield;

  static constexpr GameMechanics latest() { return GameMechanics::SWORD_SHIELD; }
};

namespace shared_name_effect {
struct KingsShield {
  static constexpr Volatile name(GameMechanics = {}) { return Volatile::KINGS_SHIELD; }
  using MainSource = pokesim::dex::KingsShield;

  static constexpr types::boost onTryMoveContactAtkBoost(GameMechanics) { return -1; }

  static void onTryHit(Simulation& simulation);
  static void onResidual(Simulation& simulation);
};
}  // namespace shared_name_effect
}  // namespace pokesim::dex
