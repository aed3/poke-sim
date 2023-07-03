#pragma once

namespace pokesim::tags::move {
// Move Category Tag
struct Physical {};
// Move Category Tag
struct Special {};
// Move Category Tag
struct Status {};

// Move Property Tag: Makes contact
struct Contact {};
// Move Property Tag: Ignores a target's substitute
struct BypassSubstitute {};
// Move Property Tag: Power is multiplied by 1.2 when used by a Pokemon with the Ability Iron Fist.
struct Punch {};

// Move Target Tag: The move can hit one Pokemon of your choice
struct AnySingleTarget {};
// Move Target Tag: The move affects the user of the move
struct Self {};

namespace effect {
// Move Effect Participant Tag: Who the effect will affect
struct MoveTarget {};
// Move Effect Participant Tag: Who created the effect
struct MoveSource {};
}  // namespace effect
}  // namespace pokesim::tags::move
