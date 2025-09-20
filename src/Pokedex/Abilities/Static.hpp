#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
namespace events {
struct Static {
  static void onDamagingHit(Simulation& simulation);
};
}  // namespace events

template <GameMechanics>
struct Static : events::Static {
  static constexpr dex::Ability name = dex::Ability::STATIC;

  struct Strings {
    static constexpr std::string_view name = "Static";
    static constexpr std::string_view smogonId = "static";
  };
};

namespace latest {
using Static = dex::Static<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex