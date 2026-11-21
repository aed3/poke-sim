#pragma once

#include <Types/Constants.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Volatile.hpp>
#include <Types/Event.hpp>
#include <Types/Move.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct Stall {
  static constexpr Volatile name(GameMechanics = {}) { return Volatile::STALL; }

  struct Strings {
    static constexpr std::string_view name() { return "Stall"; }
    static constexpr std::string_view smogonId() { return "stall"; }
  };

  static void onTryMove(Simulation& simulation);
  static void onResidual(Simulation& simulation);

  types::stallUses uses = Constants::StallMoveUses::DEFAULT;
  types::effectDuration turnsRemaining = Constants::EffectDuration::STALL_MOVE;

  constexpr bool operator==(const Stall& other) const {
    return uses == other.uses && turnsRemaining == other.turnsRemaining;
  }
};
}  // namespace pokesim::dex
   // namespace pokesim::dex
