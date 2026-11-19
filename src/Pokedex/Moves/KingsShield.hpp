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
struct KingsShield {
  static constexpr Move name(GameMechanics = {}) { return Move::KINGS_SHIELD; }
  static constexpr Type type(GameMechanics) { return Type::STEEL; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::STATUS; }

  static constexpr types::pp basePp(GameMechanics) { return 10U; }

  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::SELF; }

  struct Strings {
    static constexpr std::string_view name() { return "King's Shield"; }
    static constexpr std::string_view smogonId() { return "kingsshield"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SWORD_SHIELD; }
};
}  // namespace pokesim::dex
