#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <Utilities/Tags.hpp>
#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct Thunderbolt {
  static constexpr Move name = Move::THUNDERBOLT;
  static constexpr Type type = Type::ELECTRIC;
  static constexpr MoveCategory category = MoveCategory::SPECIAL;

  static constexpr types::baseAccuracy accuracy = 100;
  static constexpr types::basePower basePower = 90;
  static constexpr types::pp basePp = 15;

  struct targetSecondaryEffect {
    static constexpr types::baseEffectChance chance = 10;

    static constexpr Tags<status::tags::Paralysis> effectTags{};
  };

  static constexpr Tags<> moveTags{};
  static constexpr MoveTarget target = MoveTarget::ANY_SINGLE_TARGET;

  struct Strings {
    static constexpr std::string_view name = "Thunderbolt";
    static constexpr std::string_view smogonId = "thunderbolt";
  };
};

namespace latest {
using Thunderbolt = dex::Thunderbolt<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex