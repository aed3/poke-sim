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
struct Burn {
  static constexpr Status name(GameMechanics = {}) { return Status::BRN; }

  static constexpr bool isTypeImmune(GameMechanics, Type type) { return type == Type::FIRE; }

  static constexpr types::effectMultiplier physicalDamageMultiplier(GameMechanics) { return 0.5F; }
  static constexpr types::stat onResidualHpDecreaseDivisor(GameMechanics) { return 16U; }

  struct Strings {
    static constexpr std::string_view name() { return "Burn"; }
    static constexpr std::string_view smogonId() { return "brn"; }
  };

  static void onSetDamageRollModifiers(Simulation& simulation);
  static void onResidual(Simulation& simulation);
};
}  // namespace pokesim::dex
