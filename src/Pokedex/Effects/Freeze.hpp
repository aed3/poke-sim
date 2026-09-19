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
struct Freeze {
  static constexpr Status name(GameMechanics = {}) { return Status::FRZ; }

  static constexpr bool isTypeImmune(GameMechanics, Type type) { return type == Type::ICE; }

  struct Strings {
    static constexpr std::string_view name() { return "Freeze"; }
    static constexpr std::string_view smogonId() { return "frz"; }
  };

  static void onStart(Simulation& simulation);
  static void onBeforeMove(Simulation& simulation);
  static void onModifyMove(Simulation& simulation);
  static void onAfterMoveSecondary(Simulation& simulation);
  static void onDamagingHit(Simulation& simulation);
};
}  // namespace pokesim::dex
