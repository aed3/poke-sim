#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Event.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct Burn {
  static constexpr dex::Status name(GameMechanics) { return dex::Status::BRN; }

  static constexpr types::effectMultiplier physicalDamageMultiplier(GameMechanics) { return 0.5F; }
  static constexpr types::stat onResidualHpDecreaseDivisor(GameMechanics) { return 16U; }
  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Burn"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "brn"; }
  };

  static void onSetDamageRollModifiers(Simulation& simulation);
  static void onResidual(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
