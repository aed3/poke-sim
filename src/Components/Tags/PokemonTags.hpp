#pragma once

namespace pokesim::tags {
struct Pokemon {};

// Indicates the Pokemon is currently in a battle.
struct ActivePokemon {};

struct AtkStatUpdateRequired {};
struct DefStatUpdateRequired {};
struct SpdStatUpdateRequired {};
struct SpaStatUpdateRequired {};
struct SpeStatUpdateRequired {};

// Indicates the Pokemon has reached the front of the fainting queue and is running its various "on faint" and "after
// faint" events.
struct Fainting {};
struct Fainted {};

struct CanUseItem {};
struct CanRemoveItem {};
struct CanSetStatus {};
}  // namespace pokesim::tags
