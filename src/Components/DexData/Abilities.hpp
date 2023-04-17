#pragma once

#include <Components/Names/Ability.hpp>

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
