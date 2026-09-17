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
struct Poison {
  static constexpr Status name(GameMechanics = {}) { return dex::Status::PSN; }

  struct Strings {
    static constexpr std::string_view name() { return "Poison"; }
    static constexpr std::string_view smogonId() { return "psn"; }
  };

  static void onStart(Simulation& simulation);
  static void onResidual(Simulation& simulation);
};
}  // namespace pokesim::dex
