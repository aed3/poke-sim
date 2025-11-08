#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Status.hpp>
#include <Types/Event.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
namespace events {
struct Burn {
  static void onSetDamageRollModifiers(Simulation& simulation);
};
}  // namespace events

template <GameMechanics>
struct Burn : events::Burn {
  static constexpr dex::Status name = dex::Status::PAR;

  static constexpr types::effectMultiplier physicalDamageMultiplier = 0.5F;
  struct Strings {
    static constexpr std::string_view name = "Burn";
    static constexpr std::string_view smogonId = "brn";
  };
};

namespace latest {
using Burn = dex::Burn<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex