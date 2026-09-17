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
struct Sleep {
  static constexpr Status name(GameMechanics = {}) { return dex::Status::SLP; }

  struct Strings {
    static constexpr std::string_view name() { return "Sleep"; }
    static constexpr std::string_view smogonId() { return "slp"; }
  };

  static void onStart(Simulation& simulation);
  static void onBeforeMove(Simulation& simulation);
};
}  // namespace pokesim::dex
