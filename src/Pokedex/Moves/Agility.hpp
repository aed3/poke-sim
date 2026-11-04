#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct SwordsDance {
  static constexpr Move name(GameMechanics = {}) { return Move::SWORDS_DANCE; }
  static constexpr Type type(GameMechanics) { return Type::NORMAL; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::STATUS; }

  static constexpr types::pp basePp(GameMechanics) { return 20U; }

  struct targetPrimaryEffect {
    static constexpr types::boost atkBoost(GameMechanics) { return 2; }
  };

  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::SELF; }

  struct Strings {
    static constexpr std::string_view name() { return "Swords Dance"; }
    static constexpr std::string_view smogonId() { return "swordsdance"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
