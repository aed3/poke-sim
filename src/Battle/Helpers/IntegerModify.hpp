#pragma once

#include <Components/EventModifier.hpp>
#include <Types/Event.hpp>
#include <Types/MechanicConstants.hpp>

namespace pokesim {
template <typename Number1, typename Number2>
types::eventModifier fixedPointMultiply(Number1 value, Number2 multiplier) {
  types::eventModifier modifier = (types::eventModifier)(multiplier * MechanicConstants::FIXED_POINT_SCALE);
  types::eventModifier modified = value * modifier;
  types::eventModifier scaled = modified + MechanicConstants::FIXED_POINT_HALF_SCALE - 1U;
  return scaled / MechanicConstants::FIXED_POINT_SCALE;
}

template <typename Number1>
void applyChainedModifier(Number1& value, types::eventModifier eventModifier) {
  types::eventModifier modified = value * eventModifier;
  types::eventModifier scaled = modified + MechanicConstants::FIXED_POINT_HALF_SCALE - 1U;
  value = (Number1)(scaled / MechanicConstants::FIXED_POINT_SCALE);
}

template <typename Multiplier>
void chainValueToModifier(types::eventModifier& eventModifier, Multiplier multiplier) {
  types::eventModifier newModifier = (types::eventModifier)(multiplier * MechanicConstants::FIXED_POINT_SCALE);
  eventModifier =
    (eventModifier * newModifier + MechanicConstants::FIXED_POINT_HALF_SCALE) / MechanicConstants::FIXED_POINT_SCALE;
}

template <typename Multiplier>
void chainComponentToModifier(EventModifier& eventModifier, Multiplier multiplier) {
  chainValueToModifier(eventModifier.val, multiplier);
}
}  // namespace pokesim