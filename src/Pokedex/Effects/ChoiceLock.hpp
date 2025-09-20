#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Volatile.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
namespace events {
struct ChoiceLock {
  static void onDisableMove(Simulation& simulation);
};
}  // namespace events

template <GameMechanics>
struct ChoiceLock : events::ChoiceLock {
  static constexpr dex::Volatile name = dex::Volatile::CHOICE_LOCK;

  struct Strings {
    static constexpr std::string_view name = "Choice Lock";
    static constexpr std::string_view smogonId = "choicelock";
  };
};

namespace latest {
using ChoiceLock = dex::ChoiceLock<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex