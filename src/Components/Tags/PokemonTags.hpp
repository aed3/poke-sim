#pragma once

namespace pokesim::tags {
// Indicates the Pokemon is currently in a battle
struct ActivePokemon {};

struct AtkStatUpdateRequired {};
struct DefStatUpdateRequired {};
struct SpdStatUpdateRequired {};
struct SpaStatUpdateRequired {};
struct SpeStatUpdateRequired {};

struct Fainted {};
}  // namespace pokesim::tags
