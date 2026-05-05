#pragma once

#include <Components/Tags/MovePropertyTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Types/Stats.hpp>
#include <Utilities/Tags.hpp>
#include <string_view>

namespace pokesim::dex {
struct QuiverDance {
  static constexpr Move name(GameMechanics) { return Move::QUIVER_DANCE; }
  static constexpr Type type(GameMechanics) { return Type::BUG; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::STATUS; }

  static constexpr types::pp basePp(GameMechanics) { return 20U; }

  struct targetPrimaryEffect {
    static constexpr types::boost spaBoost(GameMechanics) { return 1; }
    static constexpr types::boost spdBoost(GameMechanics) { return 1; }
    static constexpr types::boost speBoost(GameMechanics) { return 1; }

    static constexpr Tags<> effectTags{};
  };

  static constexpr Tags<> moveTags{};
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::SELF; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Quiver Dance"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "quiverdance"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
