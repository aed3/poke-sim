#pragma once

#include <Components/MoveSlots.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/State.hpp>

namespace pokesim {
struct TransformedFrom {
  dex::Species species = dex::Species::NO_SPECIES;
  dex::Ability ability = dex::Ability::NO_ABILITY;
  types::moveSlots<MoveSlot> moves{};

  constexpr bool operator==(const TransformedFrom& other) const {
    return other.species == species && other.ability == ability && other.moves == moves;
  }
};
}  // namespace pokesim
