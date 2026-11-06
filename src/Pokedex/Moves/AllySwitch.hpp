#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/Volatile.hpp>
#include <Types/Event.hpp>
#include <Types/Move.hpp>
#include <Types/Random.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
namespace shared_name_effect {
struct AllySwitch;
}

struct AllySwitch {
  static constexpr Move name(GameMechanics = {}) { return Move::ALLY_SWITCH; }
  static constexpr Type type(GameMechanics) { return Type::PSYCHIC; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::STATUS; }

  static constexpr types::pp basePp(GameMechanics) { return 15U; }
  static constexpr types::priority priority(GameMechanics) { return 2; }

  struct targetPrimaryEffect {
    static void onHit(Simulation& simulation);
  };

  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::SELF; }

  struct Strings {
    static constexpr std::string_view name() { return "Ally Switch"; }
    static constexpr std::string_view smogonId() { return "allyswitch"; }
  };

  using Effect = shared_name_effect::AllySwitch;

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};

namespace shared_name_effect {
struct AllySwitch {
  static constexpr Volatile name(GameMechanics = {}) { return Volatile::ALLY_SWITCH; }
  using MainSource = pokesim::dex::AllySwitch;

  static constexpr types::effectDuration duration(GameMechanics) { return 2U; }

  static void onTryMove(Simulation& simulation);
  static void onResidual(Simulation& simulation);

  types::stallUses uses = Constants::StallMoveUses::DEFAULT;
  types::effectDuration turnsRemaining = Constants::EffectDuration::DEFAULT;

  constexpr bool operator==(const AllySwitch& other) const {
    return uses == other.uses && turnsRemaining == other.turnsRemaining;
  }
};
}  // namespace shared_name_effect
}  // namespace pokesim::dex
