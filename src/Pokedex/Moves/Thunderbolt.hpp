#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Components/Tags/StatusTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <string_view>

#include "../Setup/DexDataTags.hpp"

namespace pokesim::dex {
template <GameMechanics>
struct Thunderbolt {
  static constexpr Move name = Move::THUNDERBOLT;
  static constexpr Type type = Type::ELECTRIC_TYPE;
  static constexpr MoveCategory category = MoveCategory::SPECIAL;

  static constexpr types::BaseAccuracy accuracy = 100;
  static constexpr types::BasePower basePower = 90;
  static constexpr types::Pp basePp = 15;

  struct targetSecondaryEffect {
    static constexpr types::BaseEffectChance chance = 10;

    static constexpr internal::Tags<tags::status::Paralysis> effectTags{};
  };

  static constexpr internal::Tags<tags::move::AnySingleTarget> moveTags{};

  struct Strings {
    static constexpr std::string_view name = "Thunderbolt";
    static constexpr std::string_view smogonId = "thunderbolt";
  };
};

namespace latest {
using Thunderbolt = dex::Thunderbolt<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex