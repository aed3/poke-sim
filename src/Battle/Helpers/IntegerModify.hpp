#pragma once

#include <Components/EventModifier.hpp>
#include <Types/Constants.hpp>
#include <Types/Event.hpp>

namespace pokesim {
template <typename Number1, typename Number2>
constexpr types::eventModifier fixedPointMultiply(Number1 value, Number2 multiplier) {
  types::eventModifier modifier = (types::eventModifier)(multiplier * Constants::FIXED_POINT_SCALE);
  types::eventModifier modified = value * modifier;
  types::eventModifier scaled = modified + Constants::FIXED_POINT_HALF_SCALE - 1U;
  return scaled / Constants::FIXED_POINT_SCALE;
}

template <typename Number>
constexpr Number applyChainedModifier(Number value, types::eventModifier eventModifier) {
  types::eventModifier modified = value * eventModifier;
  types::eventModifier scaled = modified + Constants::FIXED_POINT_HALF_SCALE - 1U;
  return (Number)(scaled / Constants::FIXED_POINT_SCALE);
}

template <typename Numerator>
constexpr types::eventModifier chainValueToModifier(
  types::eventModifier eventModifier, Numerator numerator, types::eventModifier denominator = 1U) {
  types::eventModifier newModifier = (types::eventModifier)(numerator * Constants::FIXED_POINT_SCALE / denominator);
  return (eventModifier * newModifier + Constants::FIXED_POINT_HALF_SCALE) / Constants::FIXED_POINT_SCALE;
}

template <typename Numerator>
void chainComponentToModifier(
  EventModifier& eventModifier, Numerator numerator, types::eventModifier denominator = 1U) {
  eventModifier.val = chainValueToModifier(eventModifier.val, numerator, denominator);
}
}  // namespace pokesim
