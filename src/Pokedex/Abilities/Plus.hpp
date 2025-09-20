#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Event.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
namespace events {
struct Plus {
  static void onModifySpA(Simulation& simulation);
};
}  // namespace events

template <GameMechanics>
struct Plus : events::Plus {
  static constexpr dex::Ability name = dex::Ability::PLUS;

  static constexpr types::effectMultiplier onModifySpaModifier = 1.5F;
  struct Strings {
    static constexpr std::string_view name = "Plus";
    static constexpr std::string_view smogonId = "plus";
  };
};

namespace latest {
using Plus = dex::Plus<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex