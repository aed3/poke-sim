#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}

namespace pokesim::dex {
namespace internal {
struct StaticEvents {
  static void onDamagingHit(Simulation& simulation);
};
}  // namespace internal

template <GameMechanics>
struct Static : internal::StaticEvents {
  static constexpr dex::Ability name = dex::Ability::STATIC;

  struct Strings {
    static constexpr std::string_view name = "Static";
    static constexpr std::string_view smogonId = "static";
  };
};
}  // namespace pokesim::dex