#pragma once
#include <Types/Entity.hpp>
#include <Types/Enums/Nature.hpp>
// TODO(aed3): Make this auto generated

namespace pokesim::nature::tags {
struct Adamant {};
struct Bashful {};
struct Bold {};
struct Brave {};
struct Calm {};
struct Careful {};
struct Docile {};
struct Gentle {};
struct Hardy {};
struct Hasty {};
struct Impish {};
struct Jolly {};
struct Lax {};
struct Lonely {};
struct Mild {};
struct Modest {};
struct Naive {};
struct Naughty {};
struct Quiet {};
struct Quirky {};
struct Rash {};
struct Relaxed {};
struct Sassy {};
struct Serious {};
struct Timid {};

// Assigns a nature's tag to a handle
void enumToTag(dex::Nature nature, types::handle handle);
}  // namespace pokesim::nature::tags
