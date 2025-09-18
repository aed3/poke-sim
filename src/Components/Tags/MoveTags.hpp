#pragma once

namespace pokesim::move {
namespace tags {
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
// Move Property Tag: Move hits 2, 3, 4, or 5 times depending on random chance, items (i.e. Loaded Dice), and abilities
// (i.e. Skill Link)
struct VariableHitCount {};
// Move Property Tag: A multi-hit move where each hit checks accuracy (i.e. Triple Kick)
struct AccuracyDependentHitCount {};

struct Disabled {};
}  // namespace tags

namespace effect::tags {
// Move Effect Participant Tag: Who the effect will affect
struct MoveTarget {};
// Move Effect Participant Tag: Who created the effect
struct MoveSource {};

struct Primary {};
struct Secondary {};
}  // namespace effect::tags
}  // namespace pokesim::move
