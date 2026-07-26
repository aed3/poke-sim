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
}

namespace pokesim::dex {
struct SpiritShackle {
  static constexpr Move name(GameMechanics) { return Move::SPIRIT_SHACKLE; }
  static constexpr Type type(GameMechanics) { return Type::GHOST; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::PHYSICAL; }

  static constexpr types::baseAccuracy accuracy(GameMechanics) { return 100U; }
  static constexpr types::basePower basePower(GameMechanics) { return 80U; }
  static constexpr types::pp basePp(GameMechanics) { return 10U; }

  struct targetSecondaryEffect {
    static constexpr types::percentChance chance(GameMechanics) { return 100U; }
    static void onHit(Simulation& Simulation);
  };

  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ANY_SINGLE_TARGET; }

  struct Strings {
    static constexpr std::string_view name() { return "Spirit Shackle"; }
    static constexpr std::string_view smogonId() { return "spiritshackle"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
