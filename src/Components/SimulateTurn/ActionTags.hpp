#pragma once

namespace pokesim::action::tags {
struct Move {};
struct Item {};

struct BeforeTurn {};
struct Residual {};

struct Current {};

struct Switch {};

struct NotFaintedActiveSwitch {};
struct ActiveSwitch {};

// Action Tag: When Revival Blessing is being used on a fainted party member
struct RevivalBlessing {};
// Action Tag: When a mega evolution is activating
struct MegaEvolve {};
// Action Tag: When a primal reversion is activating
struct PrimalRevert {};
// Action Tag: When a Pokemon dynamaxing
struct Dynamax {};
// Action Tag: When a Pokemon is terastallizing
struct Terastallize {};
}  // namespace pokesim::action::tags
