#pragma once

#include <Components/Tags/MovePropertyTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveProperty.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Event.hpp>
#include <Types/Move.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct KnockOff {
  static constexpr Move name(GameMechanics) { return Move::KNOCK_OFF; }
  static constexpr Type type(GameMechanics) { return Type::DARK; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::PHYSICAL; }

  static constexpr types::baseAccuracy accuracy(GameMechanics) { return 100U; }
  static constexpr types::basePower basePower(GameMechanics) { return 65U; }
  static constexpr types::pp basePp(GameMechanics) { return 20U; }

  static constexpr MoveProperty moveProperties(GameMechanics) { return MoveProperty::CONTACT; }
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ANY_SINGLE_TARGET; }

  static constexpr types::effectMultiplier onBasePowerMultiplier(GameMechanics) { return 1.5F; }
  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Knock Off"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "knockoff"; }
  };

  static void onBasePower(Simulation& simulation);
  static void onAfterHit(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
