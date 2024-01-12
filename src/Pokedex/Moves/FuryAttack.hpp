#pragma once

#include <Components/Tags/MoveTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
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

  static constexpr types::BaseAccuracy accuracy = 85;
  static constexpr types::BasePower basePower = 15;
  static constexpr types::Pp basePp = 20;
  static constexpr types::MoveHits minHits = 2, maxHits = 5;

  static constexpr internal::Tags<move::tags::AnySingleTarget, move::tags::Contact> moveTags{};

  struct Strings {
    static constexpr std::string_view name = "Fury Attack";
    static constexpr std::string_view smogonId = "furyattack";
  };
};

namespace latest {
using FuryAttack = dex::FuryAttack<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex