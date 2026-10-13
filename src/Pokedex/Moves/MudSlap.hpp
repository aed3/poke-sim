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

namespace pokesim::dex {
struct MudSlap {
  static constexpr Move name(GameMechanics = {}) { return Move::MUD_SLAP; }
  static constexpr Type type(GameMechanics) { return Type::GROUND; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::SPECIAL; }

  static constexpr types::baseAccuracy accuracy(GameMechanics) { return 100U; }
  static constexpr types::basePower basePower(GameMechanics) { return 20U; }
  static constexpr types::pp basePp(GameMechanics) { return 10U; }

  struct targetSecondaryEffect {
    static constexpr types::percentChance chance(GameMechanics) { return 100U; }
    static constexpr types::boost accBoost(GameMechanics) { return -1; }
  };

  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ANY_SINGLE_TARGET; }

  struct Strings {
    static constexpr std::string_view name() { return "Mud-Slap"; }
    static constexpr std::string_view smogonId() { return "mudslap"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
