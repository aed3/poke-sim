#pragma once

namespace pokesim::tags {
struct Pokemon {};

// Indicates the Pokemon is currently in a battle
struct ActivePokemon {};

struct AtkStatUpdateRequired {};
struct DefStatUpdateRequired {};
struct SpdStatUpdateRequired {};
struct SpaStatUpdateRequired {};
struct SpeStatUpdateRequired {};

struct Fainted {};

struct CanUseItem {};
struct CanRemoveItem {};
struct CanSetStatus {};
}  // namespace pokesim::tags
