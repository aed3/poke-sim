#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Volatile.hpp>
#include <Types/Random.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct Flinch {
  static constexpr Volatile name(GameMechanics) { return dex::Volatile::FLINCH; }

  struct Strings {
    static constexpr std::string_view name() { return "Flinch"; }
    static constexpr std::string_view smogonId() { return "flinch"; }
  };

  static void onBeforeMove(Simulation& simulation);
  static void onResidual(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
   // namespace pokesim::dex
