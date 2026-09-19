#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Event.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct Poison {
  static constexpr Status name(GameMechanics = {}) { return Status::PSN; }

  static constexpr bool isTypeImmune(GameMechanics, Type type) { return type == Type::POISON || type == Type::STEEL; }

  struct Strings {
    static constexpr std::string_view name() { return "Poison"; }
    static constexpr std::string_view smogonId() { return "psn"; }
  };

  static void onStart(Simulation& simulation);
  static void onResidual(Simulation& simulation);
};
}  // namespace pokesim::dex
