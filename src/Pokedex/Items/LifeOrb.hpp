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
namespace events {
struct LifeOrb {
  static void onModifyDamage(Simulation& simulation);
  static void onAfterMoveUsed(Simulation& simulation);
};
}  // namespace events

template <GameMechanics>
struct LifeOrb {
  static constexpr dex::Item name = dex::Item::LIFE_ORB;

  static constexpr types::eventModifier onModifyDamageNumerator = 5324U;
  static constexpr types::eventModifier onModifyDamageDenominator = 4096U;

  static constexpr types::stat onAfterMoveUsedHpDecreaseDivisor = 10U;
  struct Strings {
    static constexpr std::string_view name = "Life Orb";
    static constexpr std::string_view smogonId = "lifeorb";
  };
};

namespace latest {
using LifeOrb = dex::LifeOrb<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex