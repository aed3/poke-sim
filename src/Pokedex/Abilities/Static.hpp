#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim {
class Simulation;

namespace stat {
struct EffectiveSpe;
}
}  // namespace pokesim

namespace pokesim::dex {
namespace internal {
struct StaticEvents {
  static void onDamagingHit(Simulation& simulation);
  static void onModifySpe(stat::EffectiveSpe& effectiveSpe);
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

namespace latest {
using Static = dex::Static<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex