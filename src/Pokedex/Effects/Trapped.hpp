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
struct Trapped {
  static constexpr Volatile name(GameMechanics = {}) { return Volatile::TRAPPED; }

  static constexpr bool isTypeImmune(GameMechanics, Type type) { return type == Type::GHOST; }

  struct Strings {
    static constexpr std::string_view name() { return "Trapped"; }
    static constexpr std::string_view smogonId() { return "trapped"; }
  };

  static void onResetTrappedPokemon(Simulation& simulation);
};
}  // namespace pokesim::dex
