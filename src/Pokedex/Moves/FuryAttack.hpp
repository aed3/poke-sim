#pragma once

#include <Components/Tags/MovePropertyTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveProperty.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Move.hpp>
#include <string_view>

namespace pokesim::dex {
struct FuryAttack {
  static constexpr Move name(GameMechanics) { return Move::FURY_ATTACK; }
  static constexpr Type type(GameMechanics) { return Type::NORMAL; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::PHYSICAL; }

  static constexpr types::baseAccuracy accuracy(GameMechanics) { return 85U; }
  static constexpr types::basePower basePower(GameMechanics) { return 15U; }
  static constexpr types::pp basePp(GameMechanics) { return 20U; }

  static constexpr MoveProperty moveProperties(GameMechanics) {
    return MoveProperty::CONTACT | MoveProperty::VARIABLE_HIT_COUNT;
  }
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ANY_SINGLE_TARGET; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Fury Attack"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "furyattack"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
