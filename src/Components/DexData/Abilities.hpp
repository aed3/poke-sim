#pragma once

#include <Components/Names/AbilityNames.hpp>

namespace pokesim {
struct PrimaryAbility {
  dex::Ability ability = dex::NO_ABILITY;
};

struct SecondaryAbility {
  dex::Ability ability = dex::NO_ABILITY;
};

struct HiddenAbility {
  dex::Ability ability = dex::NO_ABILITY;
};
}  // namespace pokesim
