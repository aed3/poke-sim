#pragma once

#include <Components/Tags/MovePropertyTags.hpp>
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
struct FuryAttack {
  static constexpr Move name = Move::FURY_ATTACK;
  static constexpr Type type = Type::NORMAL;
  static constexpr MoveCategory category = MoveCategory::PHYSICAL;

  static constexpr types::baseAccuracy accuracy = 85U;
  static constexpr types::basePower basePower = 15U;
  static constexpr types::pp basePp = 20U;

  static constexpr Tags<move::tags::Contact, move::tags::VariableHitCount> moveTags{};
  static constexpr MoveTarget target = MoveTarget::ANY_SINGLE_TARGET;

  struct Strings {
    static constexpr std::string_view name = "Fury Attack";
    static constexpr std::string_view smogonId = "furyattack";
  };
};

namespace latest {
using FuryAttack = dex::FuryAttack<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex