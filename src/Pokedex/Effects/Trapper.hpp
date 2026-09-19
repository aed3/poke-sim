#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Enums/Volatile.hpp>
#include <Types/Event.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct Trapper {
  static constexpr Volatile name(GameMechanics = {}) { return Volatile::TRAPPER; }

  struct Strings {
    static constexpr std::string_view name() { return "Trapper"; }
    static constexpr std::string_view smogonId() { return "trapper"; }
  };

  static void onSwitchOut(Simulation& simulation);
};
}  // namespace pokesim::dex
