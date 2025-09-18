#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Event.hpp>
#include <Types/Registry.hpp>
#include <string_view>

namespace pokesim {
struct EventModifier;
}  // namespace pokesim

namespace pokesim::dex {
namespace internal {
struct PlusEvents {
  static void onModifySpA(types::handle pokemonHandle, EventModifier& eventModifier);
};
}  // namespace internal

template <GameMechanics>
struct Plus : internal::PlusEvents {
  static constexpr dex::Ability name = dex::Ability::PLUS;

  static constexpr types::effectMultiplier onModifySpaModifier = 1.5F;
  struct Strings {
    static constexpr std::string_view name = "Plus";
    static constexpr std::string_view smogonId = "plus";
  };
};

namespace latest {
using Plus = dex::Plus<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex