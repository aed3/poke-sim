#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Volatile.hpp>
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

  types::entity val{};
};
}  // namespace pokesim::dex
