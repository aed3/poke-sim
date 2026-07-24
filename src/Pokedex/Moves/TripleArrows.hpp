#pragma once

#include <Types/Damage.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/Volatile.hpp>
#include <Types/Move.hpp>
#include <Types/Random.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}

namespace pokesim::dex {
struct TripleArrows {
  static constexpr Move name(GameMechanics) { return Move::TRIPLE_ARROWS; }
  static constexpr Type type(GameMechanics) { return Type::FIGHTING; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::PHYSICAL; }

  static constexpr types::baseAccuracy accuracy(GameMechanics) { return 100U; }
  static constexpr types::basePower basePower(GameMechanics) { return 90U; }
  static constexpr types::pp basePp(GameMechanics) { return 10U; }
  static constexpr types::critBoost critRatio(GameMechanics) { return 2U; }

  struct targetSecondaryEffect {
    struct boost {
      static constexpr types::percentChance chance(GameMechanics) { return 50U; }
      static constexpr types::boost defBoost(GameMechanics) { return -1; }
    };

    struct flinch {
      static constexpr types::percentChance chance(GameMechanics) { return 30U; }
      static constexpr Volatile volatileEffect(GameMechanics) { return Volatile::FLINCH; }
    };

    static void onSecondaryEffect(Simulation& Simulation);
  };

  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ANY_SINGLE_TARGET; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Triple Arrows"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "triplearrows"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
