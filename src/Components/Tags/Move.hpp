#pragma once

namespace pokesim::move {
// Categories

struct Physical {};
struct Special {};
struct Status {};

// Properties

struct Contact {};
struct BypassSubstitute {};
struct Punch {};

// Targets

struct AnySingleTarget {};
struct Self {};
}  // namespace pokesim::move
