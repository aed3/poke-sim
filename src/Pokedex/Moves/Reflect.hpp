#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/MoveCategory.hpp>
#include <Types/Enums/MoveProperty.hpp>
#include <Types/Enums/MoveTarget.hpp>
#include <Types/Enums/SideCondition.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Event.hpp>
#include <Types/Move.hpp>
#include <Types/Random.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
namespace shared_name_effect {
struct Reflect;
}

struct Reflect {
  static constexpr Move name(GameMechanics = {}) { return Move::REFLECT; }
  static constexpr Type type(GameMechanics) { return Type::PSYCHIC; }
  static constexpr MoveCategory category(GameMechanics) { return MoveCategory::STATUS; }

  static constexpr types::pp basePp(GameMechanics) { return 20U; }

  struct targetPrimaryEffect {
    static constexpr SideCondition sideCondition(GameMechanics) { return SideCondition::REFLECT; }
  };

  static constexpr MoveProperty properties(GameMechanics) { return MoveProperty::CAN_SNATCH; }
  static constexpr MoveTarget target(GameMechanics) { return MoveTarget::ALLY_SIDE; }

  struct Strings {
    static constexpr std::string_view name() { return "Reflect"; }
    static constexpr std::string_view smogonId() { return "reflect"; }
  };

  using Effect = shared_name_effect::Reflect;

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};

namespace shared_name_effect {
struct Reflect {
  static constexpr SideCondition name(GameMechanics = {}) { return SideCondition::REFLECT; }
  using MainSource = pokesim::dex::Reflect;

  static constexpr types::effectDuration duration(GameMechanics) { return 5U; }
  static constexpr types::effectDuration extendedDuration(GameMechanics) { return 8U; }

  static constexpr types::eventModifier onAnyModifyDamageDenominator(GameMechanics) { return 4096U; }
  static constexpr types::eventModifier onAnyModifyDamageSinglesNumerator(GameMechanics) { return 2048U; }
  static constexpr types::eventModifier onAnyModifyDamageDoublesNumerator(GameMechanics) { return 2732U; }

  static void onStart(Simulation& simulation);
  static void onAnyModifyDamage(Simulation& simulation);
  static void onResidual(Simulation& simulation);

  types::effectDuration val = Constants::EffectDuration::DEFAULT;
};
}  // namespace shared_name_effect
}  // namespace pokesim::dex
