#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Volatile.hpp>
#include <Types/Registry.hpp>
#include <string_view>

namespace pokesim {
struct ChoiceLock;
struct MoveSlots;
}  // namespace pokesim

namespace pokesim::dex {
namespace internal {
struct ChoiceLockEvents {
  static void onDisableMove(
    types::registry& registry, const pokesim::ChoiceLock& choiceLocked, const MoveSlots& moveSlots);
};
}  // namespace internal

template <GameMechanics>
struct ChoiceLock : internal::ChoiceLockEvents {
  static constexpr dex::Volatile name = dex::Volatile::CHOICE_LOCK;

  struct Strings {
    static constexpr std::string_view name = "Choice Lock";
    static constexpr std::string_view smogonId = "choicelock";
  };
};

namespace latest {
using ChoiceLock = dex::ChoiceLock<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex