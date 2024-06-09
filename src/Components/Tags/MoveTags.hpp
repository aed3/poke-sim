#pragma once

namespace pokesim::move::tags {
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

namespace effect {
// Move Effect Participant Tag: Who the effect will affect
struct MoveTarget {};
// Move Effect Participant Tag: Who created the effect
struct MoveSource {};
}  // namespace effect
}  // namespace pokesim::move::tags
