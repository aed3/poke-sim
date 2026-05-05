#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Random.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct Paralysis {
  static constexpr dex::Status name(GameMechanics) { return dex::Status::PAR; }

  static constexpr types::stat speedDividend(GameMechanics) { return 50U; }
  static constexpr types::stat speedDivisor(GameMechanics) { return 100U; }

  static constexpr types::percentChance onBeforeMoveChance(GameMechanics) { return 25U; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Paralysis"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "par"; }
  };

  static void onModifySpe(Simulation& simulation);
  static void onBeforeMove(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex
