#pragma once

#include <Components/Tags/MovePropertyTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Types/Random.hpp>
#include <Types/Stats.hpp>
#include <Utilities/Tags.hpp>
#include <string_view>

namespace pokesim::dex {
struct Moonblast {
  static constexpr Move name(GameMechanics) { return Move::MOONBLAST; }
  static constexpr Type type(GameMechanics) { return Type::FAIRY; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::SPECIAL; }

  static constexpr types::baseAccuracy accuracy(GameMechanics) { return 100U; }
  static constexpr types::basePower basePower(GameMechanics) { return 95U; }
  static constexpr types::pp basePp(GameMechanics) { return 15U; }

  struct targetSecondaryEffect {
    static constexpr types::percentChance chance(GameMechanics) { return 30U; }
    static constexpr types::boost spaBoost(GameMechanics) { return -1; }

    static constexpr Tags<> effectTags{};
  };

  static constexpr Tags<> moveTags{};
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ANY_SINGLE_TARGET; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Moonblast"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "moonblast"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
