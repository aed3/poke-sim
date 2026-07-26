#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Event.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct LifeOrb {
  static constexpr Item name(GameMechanics) { return dex::Item::LIFE_ORB; }

  static constexpr types::eventModifier onModifyDamageNumerator(GameMechanics) { return 5324U; }
  static constexpr types::eventModifier onModifyDamageDenominator(GameMechanics) { return 4096U; }

  static constexpr types::stat onAfterMoveUsedHpDecreaseDivisor(GameMechanics) { return 10U; }

  struct Strings {
    static constexpr std::string_view name() { return "Life Orb"; }
    static constexpr std::string_view smogonId() { return "lifeorb"; }
  };

  static void onModifyDamage(Simulation& simulation);
  static void onAfterMoveUsed(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
