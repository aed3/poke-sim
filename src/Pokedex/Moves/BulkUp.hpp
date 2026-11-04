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
struct BulkUp {
  static constexpr Move name(GameMechanics = {}) { return Move::BULK_UP; }
  static constexpr Type type(GameMechanics) { return Type::FIGHTING; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::STATUS; }

  static constexpr types::pp basePp(GameMechanics) { return 20U; }

  struct targetPrimaryEffect {
    static constexpr types::boost atkBoost(GameMechanics) { return 1; }
    static constexpr types::boost defBoost(GameMechanics) { return 1; }
  };

  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::SELF; }

  struct Strings {
    static constexpr std::string_view name() { return "Bulk up"; }
    static constexpr std::string_view smogonId() { return "bulkup"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
