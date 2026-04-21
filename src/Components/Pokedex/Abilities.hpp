#pragma once

#include <Types/Enums/Ability.hpp>

namespace pokesim {
// Contains one of the standard abilities a species can have.
struct PrimaryAbility {
  dex::Ability val = dex::Ability::NO_ABILITY;
};

// Contains one of the standard abilities a species can have if the species can have two standard abilities.
struct SecondaryAbility {
  dex::Ability val = dex::Ability::NO_ABILITY;
};

// Contains The hidden ability a species has.
struct HiddenAbility {
  dex::Ability val = dex::Ability::NO_ABILITY;
};
}  // namespace pokesim
