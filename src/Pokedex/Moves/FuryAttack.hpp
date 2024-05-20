#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <string_view>

#include "../Setup/DexDataTags.hpp"

namespace pokesim::dex {
template <GameMechanics>
struct FuryAttack {
  static constexpr Move name = Move::FURY_ATTACK;
  static constexpr Type type = Type::NORMAL_TYPE;
  static constexpr MoveCategory category = MoveCategory::PHYSICAL;

  static constexpr types::baseAccuracy accuracy = 85;
  static constexpr types::basePower basePower = 15;
  static constexpr types::pp basePp = 20;
  static constexpr types::moveHits minHits = 2, maxHits = 5;

  static constexpr internal::Tags<move::tags::Contact> moveTags{};
  static constexpr MoveTarget target = MoveTarget::ANY_SINGLE_TARGET;

  struct Strings {
    static constexpr std::string_view name = "Fury Attack";
    static constexpr std::string_view smogonId = "furyattack";
  };
};

namespace latest {
using FuryAttack = dex::FuryAttack<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex